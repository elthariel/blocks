#pragma once

#include "constants.hh"
#include "common/chunk.hh"
#include "seed.hh"

#include <perlinNoise2.h>

namespace blocks {
  class WorldGenerator {
  public:
    WorldGenerator(const seed &_seed);

  common::Chunk::ptr generate(common::cid _id);
  protected:
    const seed _seed;
    PerlinNoise2 _height_map;
  };
}
