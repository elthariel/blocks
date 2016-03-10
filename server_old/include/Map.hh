#pragma once

#include "common/map.hh"
#include "world_generator.hh"


namespace blocks
{
    class Map : public common::Map
    {
      public:
        Map();

        std::shared_ptr<common::Chunk> get(common::cid _cid);
        void generate(common::cid _cid);

      private:
        WorldGenerator _wg;

    };
}
