#include "bunny.hh"

#include <cstring>
#include <limits>

namespace blocks
{
  static bool reply_normal(const amqp_rpc_reply_t &reply)
  {
    return reply.reply_type == AMQP_RESPONSE_NORMAL;
  }

  BunnyServer::BunnyServer()
    : _conn(amqp_new_connection()), _sock(amqp_tcp_socket_new(_conn))
  {
    if (_sock == nullptr) throw BunnyError("Unable to create socket");
  }

  BunnyServer::~BunnyServer()
  {
    auto reply = amqp_connection_close(_conn, AMQP_REPLY_SUCCESS);
    if (reply_normal(reply))
      {
        auto status = amqp_destroy_connection(_conn);
        if (status != AMQP_STATUS_OK)
          std::cerr << "Unable to destroy connection" << std::endl;
      }
    else
      std::cerr << "Unable to close connection" << std::endl;
  }

  bool BunnyServer::connect(std::string host,
                            int port,
                            std::string login,
                            std::string pasword)
  {
    if (amqp_socket_open(_sock, host.c_str(), port)) {
      std::cout << "Unable to login to the messaging queue" << std::endl;
      return false;
    }

    auto reply = amqp_login(_conn,
                            "/",        // VHost
                            0,          // Chan max
                            1024 * 128, // framesize
                            0,          // Hearthbeat
                            AMQP_SASL_METHOD_PLAIN,
                            "guest",
                            "guest");

    if (!reply_normal(reply)) {
      std::cout << "RabbitMq: Unable to login" << std::endl;
      return false;
    }

    _connected = true;
    return true;
  }

  BunnyChannel::ptr BunnyServer::channel(unsigned int chan)
  {
    if (_channels.size() >= channels_max()) return BunnyChannel::ptr();
    if (_channels.count(chan)) return _channels[chan];

    amqp_channel_open(_conn, chan);

    auto reply = amqp_get_rpc_reply(_conn);
    if (!reply_normal(reply)) {
      std::cerr << "Unable to open channel " << chan << std::endl;
      return BunnyChannel::ptr();
    }

    return std::make_shared<BunnyChannel>(*this, chan);
  }

  size_t BunnyServer::channels_max()
  {
    auto max = amqp_get_channel_max(_conn);
    if (max)
      return max;
    else
      return std::numeric_limits<size_t>::max();
  }

  static amqp_bytes_t string_to_bytes(const std::string str)
  {
    amqp_bytes_t b;

    b.len = str.size();
    b.bytes = (void *) str.c_str();

    return b;
  }

  BunnyProperties::BunnyProperties()
  {
    std::memset((void *)&_props, 0, sizeof(_props));

    // Non Persistent message by default;
    delivery_mode(1);
  }

  void BunnyProperties::content_type(const std::string &ct)
  {
    _props.content_type = string_to_bytes(ct);
  }

  void BunnyProperties::content_encoding(const std::string &ce)
  {
    _props.content_encoding = string_to_bytes(ce);
  }

  void BunnyProperties::delivery_mode(unsigned char m)
  {
    _props.delivery_mode = m;
  }




  BunnyChannel::BunnyChannel(BunnyServer &serv, unsigned chan)
    : _serv(serv), _chan(chan)
  {
  }

  BunnyChannel::~BunnyChannel()
  {
    amqp_channel_close(_serv._conn, _chan, AMQP_REPLY_SUCCESS);

    if (!reply_normal(amqp_get_rpc_reply(_serv._conn)))
      std::cerr << "Unable to close channel " << _chan << std::endl;
  }

  bool BunnyChannel::publish(std::string exchange,
                             std::string routing_key,
                             BunnyProperties props,
                             void *data, size_t len,
                             bool mandatory)
  {
    amqp_bytes_t body;
    body.len = len;
    body.bytes = data;

    auto result = amqp_basic_publish(_serv._conn,
                                     _chan,
                                     string_to_bytes(exchange),
                                     string_to_bytes(routing_key),
                                     mandatory, false,
                                     props,
                                     body);

    return result == AMQP_STATUS_OK;
  }
}
