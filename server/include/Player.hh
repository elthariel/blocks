#pragma once

#include "TcpConnection.hh"
#include "Server.hh"
#include "position.hh"
#include "Chunk.hh"

static int NEXT_ID = 0;

namespace blocks {

    class Player
    {
      public:
        Player(TcpConnection<Server, Player>::pointer);

        int  id()           { return _id; }
        void id(int id)     { _id = id; }

        wpos pos()          { return _pos; }
        void pos(wpos &pos) { _pos = pos; }

        flatbuffers::Offset<fbs::Player> serialize(flatbuffers::FlatBufferBuilder &builder);

      private:
        TcpConnection<Server, Player>::pointer _socket;
        int _id;
        wpos _pos;
    };
}
