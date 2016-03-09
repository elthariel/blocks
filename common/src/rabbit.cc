#include "rabbit.hh"


namespace blocks
{
  RabbitServer::RabbitServer()
    : _conn(amqp_new_connection()), _sock(amqp_tcp_socket_new(_conn))
  {
    if (_sock == nullptr) throw RabbitError("Unable to create socket");
  }

  bool RabbitServer::connect(std::string host, int port,
                             std::string login, std::string pasword)
  {
    if (amqp_socket_open(_sock, host.c_str(), port))
    {
         std::cout << "Unable to login to the messaging queue" << std::endl;
         return false;
    }

    auto reply = amqp_login(_conn,
                            "/",        // VHost
                            0,          // Chan max
                            1024 * 128, // framesize
                            0,          // Hearthbeat
                            AMQP_SASL_METHOD_PLAIN,
                            "guest", "guest");

    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
      std::cout << "RabbitMq: Unable to login" << std::endl;
      return false;
    }
    _connected = true;
    return true;
  }
}
