#pragma once

#include "common/util.hh"
#include "exception.hh"

#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <amqp_framing.h>

#include <vector>

namespace blocks
{
     class RabbitError : public exception
     {
          using exception::exception;
     };

     class RabbitChannel;

     class RabbitServer: public nocopy
     {
     public:
          RabbitServer();
          ~RabbitServer();

          bool connect(std::string server,
                       int port,
                       std::string login,
                       std::string password);
          bool connected() const { return _connected; }

     protected:
          amqp_connection_state_t _conn;
          amqp_socket_t *_sock = nullptr;
          bool _connected = false;

     };


     class RabbitChannel: public nocopy
     {
     public:
          RabbitChannel() = delete;
          RabbitChannel(RabbitServer &serv, unsigned chan);

     protected:
       RabbitServer &serv;
       unsigned _chan;

     };
}
