#pragma once

#include "common/util.hh"
#include "exception.hh"

#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <amqp_framing.h>

#include <memory>
#include <vector>
#include <map>

namespace blocks
{
  class BunnyServer;

  // Base class for the exception throwed by the Bunny client
  class BunnyError : public exception
  {
    using exception::exception;
  };

  class BunnyProperties
  {
  public:
    BunnyProperties();

    operator amqp_basic_properties_t *() { return &_props; }

    void clear_flags();
    void add_flag(amqp_flags_t f);

    void content_type(const std::string &ct);
    void content_encoding(const std::string &ce);
    void delivery_mode(unsigned char);

  protected:
    amqp_basic_properties_t _props;
  };

  // Represents a channel
  class BunnyChannel : public nocopy
  {
  public:
    typedef std::shared_ptr<BunnyChannel> ptr;

    BunnyChannel() = delete;
    BunnyChannel(BunnyServer &serv, unsigned chan);
    ~BunnyChannel();

    bool publish(std::string exchange,
                 std::string routing_key,
                 BunnyProperties props,
                 void *data, size_t len,
                 bool mandatory = false);

  protected:
    BunnyServer &_serv;
    unsigned _chan;
  };

  // A connection to a server, and a container for channels
  class BunnyServer : public nocopy
  {
  public:
    friend class BunnyChannel;
    typedef std::map<unsigned, BunnyChannel::ptr> chanmap;

    BunnyServer();
    ~BunnyServer();

    bool connect(std::string server,
                 int port,
                 std::string login,
                 std::string password);
    bool connected() const { return _connected; }

    BunnyChannel::ptr channel(unsigned int chan);
    size_t channels_max();

  protected:
    chanmap _channels;
    amqp_connection_state_t _conn;
    amqp_socket_t *_sock = nullptr;
    bool _connected      = false;
  };
}
