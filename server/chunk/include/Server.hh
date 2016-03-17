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
      typedef void (Server::*handler_rpc_ptr)(fbs::RPC *, Bus<Server>::DoneCallback);
      Server();

      void on_connect_player();
      void init_handler_ptrs();
      void init_handler_rpc_ptrs();
      void dispatch(fbs::Message *);
      void dispatch_rpc(fbs::RPC *, Bus<Server>::DoneCallback);

      void on_move(fbs::Message*);
      void on_break_block(fbs::Message*);
      void on_place_block(fbs::Message*);

      void on_ask_chunk(fbs::RPC*, Bus<Server>::DoneCallback)
      ;
    private:
      std::map<int, Player *> _players;
      std::map<fbs::Action, handler_ptr> _handler_ptrs;
      std::map<fbs::AType, handler_rpc_ptr> _handler_rpc_ptrs;
      Map _map;
      Bus<Server> _bus;
    };
}

#include "Player.hh"
