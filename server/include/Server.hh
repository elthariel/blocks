#pragma once

#include <map>

#include "TcpConnection.hh"
#include "Map.hh"
#include "Protocole.hh"

namespace blocks {

    class TcpServer;
    class Player;
    class Server
    {
      public:
        typedef void (Server::*handler_ptr)(TcpConnection<Server, Player>::pointer, fbs::Message *);
        Server();

        void on_connect_player(TcpConnection<Server, Player>::pointer);
        void init_handler_ptrs();
        void dispatch(TcpConnection<Server, Player>::pointer, uint8_t*);

        void on_ask_chunk(TcpConnection<Server, Player>::pointer, fbs::Message*);
        void on_move(TcpConnection<Server, Player>::pointer, fbs::Message*);
        void on_break_block(TcpConnection<Server, Player>::pointer, fbs::Message*);
        void on_place_block(TcpConnection<Server, Player>::pointer, fbs::Message*);

      private:
        TcpServer *_tcp_server;
        std::map<int, Player *> _players;
        std::map<fbs::Action, handler_ptr> _handler_ptrs;
        Map _map;
    };
}
#include "TcpServer.hh"
#include "Player.hh"
