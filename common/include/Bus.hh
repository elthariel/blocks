#pragma once

#include <iostream>
#include <boost/thread.hpp>

#include "Protocole.hh"
#include "asiohandler.h"

#include "chunk_generated.h"

namespace blocks
{
  template <class Dispatcher>
  class Bus
  {
    public:
      typedef std::function<void(fbs::RPC *)> SuccessCallback;
      typedef std::function<void(fbs::Error *)> ErrorCallback;
      typedef std::function<void(fbs::Error *, Protocole::Message)> DoneCallback;

      Bus(std::string host, int port, std::string exchange, Dispatcher *dispatcher, bool is_rpc_receiver)
        : _exchange(exchange), _handler(_io_service), _dispatcher(dispatcher), _is_rpc_receiver(is_rpc_receiver)
      {
        try
        {
          _handler.connect(host, port);

          _connection = new AMQP::Connection(&_handler, AMQP::Login("guest", "guest"), "/");
          if (is_rpc_receiver)
            _rpc_channel = new AMQP::Channel(_connection);

          _channel = new AMQP::Channel(_connection);
          declare_exchange();
          _t = new boost::thread(boost::bind(&boost::asio::io_service::run, &_io_service));

        }
        catch (std::exception &e)
        {
          std::cerr << "Bus ctor: ERROR: " << e.what() << std::endl;
        }
      }

      ~Bus()
      {
        _t->join();
      }

      void join()
      {
        if (_t != nullptr)
          _t->join();
      }

      void declare_exchange()
      {
        AMQP::SuccessCallback success = [&]()
        {
          declare_queue();
        };

        AMQP::ErrorCallback error = [](const char *error)
        {
          std::cout << "Error declaring exchange: " << error << std::endl;
        };

        _channel->declareExchange(_exchange, AMQP::topic)
          .onSuccess(success)
          .onError(error);

      }

      void declare_queue()
      {
        AMQP::QueueCallback success = [&](const std::string &name, int msgcount, int consumercount)
        {
          _private_queue = name;
          consume();
          if (_is_rpc_receiver)
            declare_rpc_exchange();
        };

        AMQP::ErrorCallback error = [](const char *error)
        {
          std::cout << "Error declaring queue: " << error << std::endl;
        };

        _channel->declareQueue(AMQP::exclusive)
          .onSuccess(success)
          .onError(error);
      }

      void consume()
      {
        if (_started)
          return ;

        // Dequeue, as we are now fully initialized
        for (auto topic: _subscribe_event_queue)
          subscribe_events(topic);
        for (auto msg: _emit_queue)
          emit(std::get<0>(msg), std::get<1>(msg));
        for (auto rpc_: _rpc_queue)
          rpc(std::get<0>(rpc_), std::get<1>(rpc_), std::get<2>(rpc_), std::get<3>(rpc_));

        _started = true;

        auto success = [&](const AMQP::Message &message,
                           uint64_t deliveryTag,
                           bool redelivered)
        {
          //FIXME: free that buffer
          auto size = message.message().size();
          uint8_t *tmp = new uint8_t[size];
          memcpy(tmp, message.message().c_str(), size);

          if (!message.correlationID()[0])
          {
            auto msg = flatbuffers::GetMutableRoot<fbs::Message>((void *)tmp);
            _dispatcher->dispatch(msg);
          }
          else
          {
            rpc_answer(message, tmp);
          }
        };

        AMQP::ErrorCallback error = [](const char *error)
        {
          std::cout << "Error consuming: " << error << std::endl;
        };

        _channel->consume(_private_queue, AMQP::noack)
          .onReceived(success)
          .onError(error);
      }

      void declare_rpc_exchange()
      {
        AMQP::SuccessCallback success = [&]()
        {
          declare_rpc_queue();
        };

        AMQP::ErrorCallback error = [](const char *error)
        {
          std::cout << "Error declaring rpc exchange: " << error << std::endl;
        };

        _rpc_channel->setQos(1);
        _rpc_channel->declareExchange("rpc_queue", AMQP::topic)
          .onSuccess(success)
          .onError(error);

      }

      void declare_rpc_queue()
      {
        AMQP::QueueCallback success = [&](const std::string &name, int msgcount, int consumercount)
        {
          _private_rpc_queue = name;
          consume_rpc();
        };

        AMQP::ErrorCallback error = [](const char *error)
        {
          std::cout << "Error declaring rpc queue: " << error << std::endl;
        };

        _rpc_channel->declareQueue(AMQP::exclusive)
          .onSuccess(success)
          .onError(error);
      }

      void consume_rpc()
      {
        for (auto topic: _subscribe_rpc_queue)
          subscribe_rpc(topic);

        _rpc_channel->consume(_private_rpc_queue).onReceived([&](const AMQP::Message &message,
                                                                  uint64_t deliveryTag,
                                                                  bool redelivered)
          {
            auto msg = flatbuffers::GetMutableRoot<fbs::RPC>((void *)message.message().c_str());
            auto done = [&](fbs::Error *error, Protocole::Message ret)
            {
              AMQP::Envelope env((const char *)std::get<0>(ret), std::get<1>(ret));
              env.setCorrelationID(message.correlationID());

              _rpc_channel->publish("", message.replyTo(), env);
              _rpc_channel->ack(deliveryTag);
            };

            _dispatcher->dispatch_rpc(msg, done);
          });
      }

      template <class Queue, class Data>
      bool enqueue(Queue &queue, Data data)
      {
        if (!_private_queue.c_str()[0])
        {
          queue.push_back(data);
          return true;
        }
        return false;
      }

      void subscribe_events(std::string topic)
      {
        if (enqueue(_subscribe_event_queue, topic)) return;

        _channel->bindQueue(_exchange, _private_queue, topic);
      }

      void subscribe_rpc(std::string topic)
      {
        if (_rpc_channel == nullptr)
        {
          std::cout << "Cannot subscribe to RPC when its not initialized (_is_rpc_receiver == false)" << std::endl;
          return ;
        }

        if (!_private_rpc_queue.c_str()[0])
        {
          _subscribe_rpc_queue.push_back(topic);
          return;
        }

        _rpc_channel->bindQueue("rpc_queue", _private_rpc_queue, topic);
      }

      void emit(std::string dest, Protocole::Message msg)
      {
        if (enqueue(_emit_queue, std::make_tuple(dest, msg))) return;

        AMQP::Envelope env((const char *)std::get<0>(msg), std::get<1>(msg));
        env.setReplyTo(_private_queue);
        env.setCorrelationID("");
        _channel->publish(_exchange, dest, (const char *)std::get<0>(msg), std::get<1>(msg));
      }

      template <class T>
      void emit(std::string dest, fbs::Action action, T obj)
      {
        auto msg = Protocole::create_message(action, obj);
        if (enqueue(_emit_queue, std::make_tuple(dest, msg))) return;

        emit(dest, msg);
      }

      void rpc(std::string routing, Protocole::Message msg, SuccessCallback success, ErrorCallback error)
      {
        if (enqueue(_rpc_queue, std::make_tuple(routing, msg, success, error))) return;

        char nb[8];
        AMQP::Envelope env((const char *)std::get<0>(msg), std::get<1>(msg));

        sprintf(nb, "%d", _correlation_id);
        _correlation_id++;
        env.setCorrelationID(nb);
        env.setReplyTo(_private_queue);

        _rpc_waiting.insert(std::make_pair(nb, std::make_pair(success, error)));
        _channel->publish("rpc_queue",routing,env);
      }

      template <class T>
      void rpc(std::string routing, T obj, SuccessCallback success, ErrorCallback error)
      {
        auto msg = Protocole::create_rpc(obj);
        if (enqueue(_rpc_queue, std::make_tuple(routing, msg, success, error))) return;

        rpc(routing, msg, success, error);
      }

      void rpc_answer(const AMQP::Message &msg, uint8_t *buffer)
      {
        auto callbacks = _rpc_waiting[msg.correlationID()];
        auto it = _rpc_waiting.find(msg.correlationID());
        _rpc_waiting.erase(it);
        auto success = std::get<0>(callbacks);
        blocks::fbs::RPC *rpc = flatbuffers::GetMutableRoot<blocks::fbs::RPC>(buffer);
        success(rpc);
      }

    private:
      std::string _private_queue = "";
      std::string _private_rpc_queue = "";
      bool _is_rpc_receiver = false;
      Dispatcher *_dispatcher;
      boost::asio::io_service _io_service;
      AMQP::Connection *_connection;
      AMQP::Channel *_channel = nullptr;
      AMQP::Channel *_rpc_channel = nullptr;
      boost::thread *_t = nullptr;
      std::string _exchange;
      AsioHandler _handler;
      bool _started = false;
      std::vector<std::string> _subscribe_event_queue;
      std::vector<std::string> _subscribe_rpc_queue;
      std::vector<std::tuple<std::string, Protocole::Message>> _emit_queue;
      std::vector<std::tuple<std::string, Protocole::Message, SuccessCallback, ErrorCallback>> _rpc_queue;
      std::map<std::string, std::pair<SuccessCallback, ErrorCallback>> _rpc_waiting;
      unsigned _correlation_id = 0;
  };
}
