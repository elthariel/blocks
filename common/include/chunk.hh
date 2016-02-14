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


    const static size_t _size = Power<consts::chunk_size, 3>::value;
    static size_t size() { return _size; }


    Block &operator[](const cpos &p) {
      return _blocks[p.to_idx() % _size];
    }

    const Block &operator[](const cpos &p) const {
      return _blocks[p.to_idx() % _size];
    }

    Block &operator[] (uint64_t idx) {
      return _blocks[idx % _size];
    }

    const Block &operator[](uint64_t idx) const {
      return _blocks[idx % _size];
    }


  protected:
    Block _blocks[_size];
  };
}
