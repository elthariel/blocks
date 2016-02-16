#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

class Server;
#include "TcpConnection.hh"

using boost::asio::ip::tcp;


class TcpServer
{
  public:
    TcpServer(boost::asio::io_service&, Server*);

  private:
    int  _port = 3000;
    void start_accept();
    void handle_accept(TcpConnection::pointer, const boost::system::error_code&);

    tcp::acceptor _acceptor;
    Server *_server;
};
#include "Server.hh"
