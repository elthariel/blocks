
#include "common/map.hh"

namespace blocks {
  namespace common {
    std::shared_ptr<Chunk> Map::get(cid _cid) {
      return _map[_cid];
    }

    void Map::set(cid _cid, Chunk::ptr _chunk) {
      _map[_cid] = _chunk;
    }

    bool Map::exists(cid _cid) {
      return _map.count(_cid) == 1;
    }
  }
}
