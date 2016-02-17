
#include "common/map.hh"

namespace blocks {
  namespace common {
    Chunk::ptr Map::get(const cid &_cid) const
    {
      if (_map.count(_cid))
        return _map.at(_cid);
      else
        // Null pointer
        return Chunk::ptr();
    }

    void Map::set(const cid &_cid, Chunk::ptr _chunk)
    {
      _map[_cid] = _chunk;
    }

    bool Map::exists(const cid &_cid) const
    {
      return _map.count(_cid) == 1;
    }
  }
}
