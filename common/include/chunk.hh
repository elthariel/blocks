#pragma once

#include "block.hh"
#include "chunk_generated.h"
#include "constants.hh"
#include "meta.hh"
#include "multi_array.hh"
#include "position.hh"

#include <memory>

namespace blocks {
  class Chunk {
  public:
    typedef std::shared_ptr<Chunk> ptr;

    const static size_t _flat_size = Power<consts::chunk_size, 3>::value;
    static size_t size() { return consts::chunk_size; }
    static size_t flat_size() { return _flat_size; }

    Chunk(cid id) : _id(id)
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

  protected:
    cid _id;
    Block _blocks[_flat_size];
  };
}
