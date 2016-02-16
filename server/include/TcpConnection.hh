#pragma once

#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "chunk_generated.h"
#include "CircularBuffer.hh"

class Server;

using boost::asio::ip::tcp;

class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
{
  public:
    typedef boost::shared_ptr<TcpConnection> pointer;

    static pointer create(boost::asio::io_service&, Server*);

    tcp::socket& socket();
    void start();
    void read();

  private:
    TcpConnection(boost::asio::io_service&, Server*);

    void handle_write(const boost::system::error_code&, size_t);
    void handleReadHeader(const boost::system::error_code&, uint8_t*);


    Server *_server;
    tcp::socket socket_;
    std::string message_;
    CircularBuffer _cb;
};

#include "Server.hh"
