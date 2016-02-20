#include "chunk_generated.h"
#include "world_generator.hh"

#include <boost/timer/timer.hpp>
#include <cmath>
#include <vector>
#include <iostream>

namespace blocks {
  WorldGenerator::WorldGenerator(const seed &_seed)
  : _seed(_seed), _height_map(16, 16, 256, _seed.get_64(0))
  {
  }

  common::Chunk::ptr WorldGenerator::generate(common::cid _id) {
    // boost::timer::auto_cpu_timer t(6, " generation: %w seconds\n");
    common::Chunk::ptr chunk = std::make_shared<common::Chunk>(_id);

    for(size_t x = 0; x < consts::chunk_size; x++) {
      for(size_t y = 0; y < consts::chunk_size; y++) {
        for(size_t z = 0; z < consts::chunk_size; z++) {
          common::cpos c(x, y, z);
          common::wpos w(_id, c);

          if (w.z() > _height_map(w.x(), w.y()) * 8 + 10) {
            if (w.z() > 0)
            {
              (*chunk)[c.to_idx()].id(0);
              (*chunk)[c.to_idx()].air(true);
            }
            else
              (*chunk)[c.to_idx()].id(2);
          } else {
            (*chunk)[c.to_idx()].id(1);
          }
        }
      }
    }

    return chunk;
  }
}