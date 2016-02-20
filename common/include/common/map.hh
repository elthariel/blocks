#pragma once

#include "util.hh"
#include "common/position.hh"
#include "chunk.hh"

#include <map>
#include <memory>
#include <tuple>

namespace blocks {
  namespace common {
    typedef std::map<cid, Chunk::ptr> chunk_map;

    class Map : public nocopy
    {
    public:
      Chunk::ptr get(const cid &) const;
      void set(const cid &, Chunk::ptr);
      bool exists(const cid &) const;
    protected:
      chunk_map _map;
    };
  }
}
