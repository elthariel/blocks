#pragma once

#include "position.hh"
#include "chunk.hh"

#include <map>
#include <memory>
#include <tuple>

namespace blocks {
  namespace common {
    typedef std::map<cid, std::shared_ptr<Chunk>> chunk_map;

    class Map {
      Chunk::ptr get(cid);
      void set(cid, Chunk::ptr);
      bool exists(cid);
    protected:
      chunk_map _map;
    };
  }
}
