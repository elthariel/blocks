#pragma once

#include <iostream>
#include <boost/thread.hpp>

#include "Protocole.hh"
#include "asiohandler.h"

namespace blocks
{
  template <class Dispatcher>
  class Bus
  {
    public:
      typedef std::function<void(fbs::RPC *)> SuccessCallback;
      typedef std::function<void(fbs::Error *)> ErrorCallback;

      Bus(std::string host, int port, std::string exchange, Dispatcher *dispatcher)
        : _exchange(exchange), _handler(_io_service), _dispatcher(dispatcher)
      {
        try
        {
          std::cout << "DISPATCHER" << dispatcher <<  std::endl;
          _handler.connect(host, port);

          _connection = new AMQP::Connection(&_handler, AMQP::Login("guest", "guest"), "/");
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

        for (auto topic: _subscribe_queue)
          subscribe(topic);
        for (auto msg: _emit_queue)
          emit(std::get<0>(msg), std::get<1>(msg));
        for (auto rpc_: _rpc_queue)
          rpc(std::get<0>(rpc_), std::get<1>(rpc_), std::get<2>(rpc_));

        _started = true;

        auto success = [&](const AMQP::Message &message,
                           uint64_t deliveryTag,
                           bool redelivered)
        {
          if (message.correlationID()[0] != '1')
            _dispatcher->dispatch((uint8_t *)message.message().c_str());
          else
            rpc_answer(message);
        };

        AMQP::ErrorCallback error = [](const char *error)
        {
          std::cout << "Error consuming: " << error << std::endl;
        };

        _channel->consume(_private_queue, AMQP::noack)
          .onReceived(success)
          .onError(error);
      }

      void subscribe(std::string topic)
      {
        if (!_private_queue.c_str()[0])
        {
          _subscribe_queue.push_back(topic);
          return ;
        }

        _channel->bindQueue(_exchange, _private_queue, topic);
      }

      void emit(std::string dest, Protocole::Message msg)
      {
        if (!_private_queue.c_str()[0])
        {
          _emit_queue.push_back(std::pair<std::string, Protocole::Message>(dest, msg));
          return ;
        }
        _channel->publish(_exchange, dest, (const char *)std::get<0>(msg), std::get<1>(msg));
      }

      void rpc(Protocole::Message msg, SuccessCallback success, ErrorCallback error)
      {
        if (!_private_queue.c_str()[0])
        {
          _rpc_queue.push_back(std::make_tuple(msg, success, error));
          return ;
        }
        AMQP::Envelope env((const char *)std::get<0>(msg), std::get<1>(msg));
        env.setCorrelationID("1");
        env.setReplyTo(_private_queue);
        _rpc_waiting.push(std::pair<SuccessCallback, ErrorCallback>(success, error));
        _channel->publish("","rpc_queue",env);
      }

      void rpc_answer(const AMQP::Message &msg)
      {
        std::pair<SuccessCallback, ErrorCallback> callbacks = _rpc_waiting.front();
        _rpc_waiting.pop();
        auto success = std::get<0>(callbacks);
        blocks::fbs::RPC *rpc = flatbuffers::GetMutableRoot<blocks::fbs::RPC>((uint8_t *)msg.message().c_str());
        success(rpc);
      }

    private:
      std::string _private_queue = "";
      Dispatcher *_dispatcher;
      boost::asio::io_service _io_service;
      AMQP::Connection *_connection;
      AMQP::Channel *_channel;
      boost::thread *_t = nullptr;
      std::string _exchange;
      AsioHandler _handler;
      bool _started = false;
      std::vector<std::string> _subscribe_queue;
      std::vector<std::pair<std::string, Protocole::Message>> _emit_queue;
      std::vector<std::tuple<Protocole::Message, SuccessCallback, ErrorCallback>> _rpc_queue;
      std::queue<std::pair<SuccessCallback, ErrorCallback>> _rpc_waiting;
  };
}
