#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "TcpConnection.hh"
#include "Player.hh"
using boost::asio::ip::tcp;

namespace blocks {

    class Server;
    class TcpServer
    {
      public:
        TcpServer(boost::asio::io_service&, Server*);

        void send_all_except(TcpConnection<Server, Player>::pointer, std::tuple<uint8_t *, uint64_t>);

      private:
        int  _port = 3000;
        void start_accept();
        void handle_accept(TcpConnection<Server, Player>::pointer, const boost::system::error_code&);

        std::vector<TcpConnection<Server, Player>::pointer> _sockets;
        tcp::acceptor _acceptor;
        Server *_server;
    };
}
#include "Server.hh"
#include "Player.hh"
