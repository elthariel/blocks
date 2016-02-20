#pragma once

#include "common/position.hh"

namespace blocks
{
  namespace events
  {
    struct load_chunk
    {
      common::cid id;

      load_chunk(const common::cid &_id) : id(_id) {}
    };
  }
}
