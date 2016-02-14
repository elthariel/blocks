#pragma once

#include "constants.hh"
#include "chunk.hh"
#include "seed.hh"

#include <perlinNoise2.h>

namespace blocks {
  class WorldGenerator {
  public:
    WorldGenerator(const seed &_seed);

    Chunk::ptr generate(cid _id);
  protected:
    const seed _seed;
    PerlinNoise2 _height_map;
  };
}
