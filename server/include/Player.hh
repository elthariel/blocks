#pragma once

#include "TcpConnection.hh"
#include "Server.hh"
#include "position.hh"
#include "Chunk.hh"


// struct HasId
// {
//   static int next_id() { return __id++; }
//   static int __id;
// };
//
// int HasId::__id = 0;

static int NEXT_ID = 0;
namespace blocks {

    class Player
    {
      public:
        Player(TcpConnection<Server, Player>::pointer);

        int  id()       { return _id; }
        void id(int id) { _id = id; }

      private:
        TcpConnection<Server, Player>::pointer _socket;
        int _id;
        blocks::wpos _pos;
    };
}
