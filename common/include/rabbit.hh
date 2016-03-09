#pragma once

#include "common/util.hh"
#include "exception.hh"

#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <amqp_framing.h>

namespace blocks
{
     class RabbitError : public exception
     {
          using exception::exception;
     };

     class Rabbit: public nocopy
     {
     public:
       Rabbit() = delete;
          Rabbit();

          bool connect(std::string server,
                       int port,
                       std::string login,
                       std::string password);

     protected:
          amqp_connection_state_t _conn;
          amqp_socket_t *_sock = nullptr;
          bool _connected = false;

     };
}
