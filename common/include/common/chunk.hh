#pragma once

#include "block.hh"
#include "chunk_generated.h"
#include "constants.hh"
#include "meta.hh"
#include "multi_array.hh"
#include "common/position.hh"

#include <memory>

namespace blocks {
    namespace common {
      class Chunk {
      public:
        typedef std::shared_ptr<Chunk> ptr;

        const static size_t _flat_size = Power<consts::chunk_size, 3>::value;
        static size_t size() { return consts::chunk_size; }
        static size_t flat_size() { return _flat_size; }

        Chunk(common::cid id) : _id(id)
        {}

        const cid& id() const { return _id; }

        Block &operator[](const cpos &p) {
          return at(p);
        }

        const Block &operator[](const cpos &p) const {
          return at(p);
        }

        Block &operator[] (uint64_t idx) {
          return at(idx);
        }

        const Block &operator[](uint64_t idx) const {
          return at(idx);
        }

        Block &at(const cpos &p) {
          return _blocks[p.to_idx() % _flat_size];
        }

        const Block &at(const cpos &p) const {
          return _blocks[p.to_idx() % _flat_size];
        }

        Block &at (uint64_t idx) {
          return _blocks[idx % _flat_size];
        }

        const Block &at(uint64_t idx) const {
          return _blocks[idx % _flat_size];
        }

        static ptr deserialize(fbs::Chunk const *chunk)
        {
            auto blocks = chunk->blocks();
            auto pos = chunk->cid();

            cid _cid(pos->x(), pos->y(), pos->z());

            auto res = ptr(new Chunk(_cid));
            auto size = blocks->Length();

            for (int i = 0; i < size; i++)
            {
                // std::cout << "DESERIALIZE ITERATE " << i << std::endl;
                auto block = blocks->Get(i);
                res->at(i) = Block(block->id(), block->variant(), block->air(), block->transparent(), block->light());
            }
            return res;
        }

        flatbuffers::Offset<fbs::Chunk> serialize(flatbuffers::FlatBufferBuilder &builder)
        {
            std::vector<flatbuffers::Offset<fbs::Block>> blocks_vector;
            for (auto block: _blocks)
              blocks_vector.push_back(block.serialize(builder));

            auto blocks = builder.CreateVector(blocks_vector);
            auto pos = fbs::Pos(_id.x(), _id.y(), _id.z());
            return fbs::CreateChunk(builder, 0, 8, &pos, blocks);
        }

        inline fbs::AType atype() { return fbs::AType::AType_Chunk; }

      protected:
        cid _id;
        Block _blocks[_flat_size];
      };
  }
}
