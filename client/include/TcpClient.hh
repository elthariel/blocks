#pragma once

#include "TcpConnection.hh"

#include <boost/thread.hpp>

namespace blocks {
  namespace systems { struct Network; }

  class TcpClient {
  public:
    typedef TcpConnection<systems::Network, systems::Network> connection;

    TcpClient(systems::Network &net, std::string ip, std::string port);
    ~TcpClient();

    void connect(boost::asio::ip::tcp::endpoint& endpoint);
    void handle_connect(const boost::system::error_code& error);
  private:
    boost::thread *_t;
    boost::asio::io_service _io_service;

    connection::pointer _socket;
    systems::Network &_network;
  };
}

#include "systems/network.hh"
