#pragma once

#include <map>

#include "Map.hh"
#include "Protocole.hh"
#include "Bus.hh"

namespace blocks {

    class Player;
    class Server
    {
    public:
      typedef void (Server::*handler_ptr)(fbs::Message *);
      Server();

      void on_connect_player();
      void init_handler_ptrs();
      void dispatch(uint8_t*);

      void on_ask_chunk(fbs::Message*);
      void on_move(fbs::Message*);
      void on_break_block(fbs::Message*);
      void on_place_block(fbs::Message*);

    private:
      std::map<int, Player *> _players;
      std::map<fbs::Action, handler_ptr> _handler_ptrs;
      Map _map;
      Bus<Server> _bus;
    };
}

#include "Player.hh"
