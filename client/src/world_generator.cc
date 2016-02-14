#include "chunk_generated.h"
#include "world_generator.hh"

#include <cmath>
#include <vector>

namespace blocks {
  WorldGenerator::WorldGenerator(const seed &_seed)
  : _seed(_seed), _height_map(50, 50, 256, _seed.get_64(0))
  {
  }

  Chunk::ptr WorldGenerator::generate(cid _id) {
    Chunk::ptr chunk = std::make_shared<Chunk>();

    for(size_t x = 0; x < consts::chunk_size; x++) {
      for(size_t y = 0; y < consts::chunk_size; y++) {
        for(size_t z = 0; z < consts::chunk_size; z++) {
          cpos c(x, y, z);
          wpos w(_id, c);

          if (w.z() > _height_map(w.x(), w.y()) * 32) {
            (*chunk)[c.to_idx()].id(0);
            (*chunk)[c.to_idx()].air(true);
          } else {
            (*chunk)[c.to_idx()].id(1);
          }
        }
      }
    }
  }
}
