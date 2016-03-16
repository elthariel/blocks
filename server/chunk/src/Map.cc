#include "Map.hh"

#include "seed.hh"
#include "Chunk.hh"

namespace blocks
{
    Map::Map()
      : _wg(seed::from_file("/dev/urandom")) {}

    std::shared_ptr<common::Chunk> Map::get(common::cid _cid) {
      if (!exists(_cid))
        generate(_cid);

      return _map[_cid];
    }

    void Map::generate(common::cid _cid)
    {
      set(_cid, _wg.generate(_cid));
    }

}
