#pragma once

#include <boost/thread.hpp>

#include "TcpConnection.hh"

namespace blocks {
    class Game;
    class TcpClient {
      public:
        TcpClient(char *ip, char *port, Game *game);
        ~TcpClient();

        void connect(boost::asio::ip::tcp::endpoint& endpoint);
        void handle_connect(const boost::system::error_code& error);

    private:
        boost::thread *_t;
        boost::asio::io_service _io_service;
        TcpConnection<Game, Game>::pointer _socket;
        Game *_game;

    };
}
#include "game.hh"
