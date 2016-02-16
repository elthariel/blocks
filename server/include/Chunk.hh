#pragma once

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
namespace blocks {

    class BlockServer : public fbs::Block
    {
      using fbs::Block::Block;
    };

    class ChunkServer : public fbs::Chunk, public Chunk
    {
      public:
        // Chunk() {}
        // using fbs::Chunk::Chunk;

      public:
          uint8_t *serialize() const {
            flatbuffers::FlatBufferBuilder builder;

            std::vector<fbs::Block> blocks_vector;
            for (auto block: _blocks)
              blocks_vector.push_back(block.serialize());

            auto blocks = builder.CreateVectorOfStructs(blocks_vector);
            auto pos = fbs::Pos(_id.x(), _id.y(), _id.z());
            auto chunk = fbs::CreateChunk(builder, 10, 1, &pos, blocks);
            builder.Finish(chunk);

            return builder.GetBufferPointer();
          }

    };
}