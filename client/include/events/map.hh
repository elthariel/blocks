#pragma once

#include "common/position.hh"

namespace blocks
{
  namespace events
  {
    struct chunk_requested
    {
      common::cid id;

      chunk_requested(const common::cid &_id) : id(_id) {}
    };
  }
}
