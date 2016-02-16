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

        std::tuple<uint8_t *, size_t> serialize();

        // static std::tuple<uint8_t *, size_t> create_message(fbs::Type type, fbs::Action action, flatbuffers::Offset<fbs::Player> body)
        // static void create_message(fbs::Type type, fbs::Action action, flatbuffers::Offset<fbs::Player> body)
        // {
        //     flatbuffers::FlatBufferBuilder builder;
        //
        //     // auto message = fbs::CreateMessage(builder, type, action, fbs::AType::AType_Player, body.Union());
        //     //
        //     // builder.Finish(message);
        //
        //     // return std::make_tuple(builder.GetBufferPointer(), builder.GetSize());
        // }

      private:
        TcpConnection<Server, Player>::pointer _socket;
        int _id;
        blocks::wpos _pos;
    };
}
