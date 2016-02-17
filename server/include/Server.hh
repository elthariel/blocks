#pragma once

#include <map>

#include "TcpConnection.hh"

namespace blocks {

    class TcpServer;
    class Player;
    class Server
    {
      public:
        Server();

        void on_connect_player(TcpConnection<Server, Player>::pointer);
        void dispatch(TcpConnection<Server, Player>::pointer, uint8_t*);

        void on_ask_chunk(TcpConnection<Server, Player>::pointer, fbs::Message*);

      private:
        TcpServer *_tcp_server;
        std::map<int, Player *> _players;
        // Map _map;
    };
}
#include "TcpServer.hh"
#include "Player.hh"
