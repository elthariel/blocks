#include "rabbit.hh"


namespace blocks
{
  Rabbit::Rabbit()
    : _conn(amqp_new_connection()), _sock(amqp_tcp_socket_new(_conn))
  {
    if (_sock == nullptr) throw RabbitError("Unable to create socket");
  }

  bool Rabbit::connect(std::string, int, std::string, std::string)
  {
    if (amqp_socket_open(_sock, host.c_str(), port))
    {
         std::cout << "Unable to login to the messaging queue" << std::endl;
         return false;
    }

    auto error = amqp_login(
      _conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
    if (error)
    {
         std::cout << "RabbitMq: Unable to login" << std::endl;
         return false;
    }

  }
}
