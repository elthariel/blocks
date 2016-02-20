#pragma once

#include "TcpConnection.hh"
#include "Server.hh"
#include "common/position.hh"
#include "Chunk.hh"

static int NEXT_ID = 0;

namespace blocks {

    // #pragma db object
    // class player_db
    // {
    // public:
    // private:
    //     int _id;
    //     pos_db _pos;
    // };

    class Player
    {
      public:
        Player(TcpConnection<Server, Player>::pointer);

        int  id()           { return _id; }
        void id(int id)     { _id = id; }

        common::wpos pos()          { return _pos; }
        void pos(common::wpos &pos) { _pos = pos; }

        // Player *deserialize(player_db&);
        flatbuffers::Offset<fbs::Player> serialize(flatbuffers::FlatBufferBuilder &builder);

      private:
        TcpConnection<Server, Player>::pointer _socket;
        int _id;
        common::wpos _pos;
    };
}
