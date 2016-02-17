#pragma once

#include <tuple>

#include "TcpConnection.hh"
#include "chunk_generated.h"
#include "common/chunk.hh"

// template <typename... T>
// class Evented
// {
//
//   public:
//     Evented(T&&... v, TcpConnection::pointer socket) : T(v)..., _socket(socket) {}
//     // Evented(TcpConnection::pointer socket) : _socket(socket) {};
//
//   // private:
//     TcpConnection::pointer _socket;
//
// };
//
// class Subject
// {
// public:
//     void attach(Observer *obs) { _observers.push_back(obs); }
//     void notify() {
//       for (auto _observers: obs) {
//           obs->update();
//       }
//     }
// private:
//     std::vector<Observer *> _observers;
// };
namespace blocks
{
    class ChunkServer : public Chunk
    {
      public:
        // Chunk() {}
        // using fbs::Chunk::Chunk;

      public:
          flatbuffers::Offset<fbs::Chunk> serialize(flatbuffers::FlatBufferBuilder &builder)
          {
              std::vector<fbs::Block> blocks_vector;
              for (auto block: _blocks)
                blocks_vector.push_back(block.serialize());

              auto blocks = builder.CreateVectorOfStructs(blocks_vector);
              auto pos = fbs::Pos(_id.x(), _id.y(), _id.z());
              return fbs::CreateChunk(builder, 0, 8, &pos, blocks);
          }
    };
}
