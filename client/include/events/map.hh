#pragma once

#include "position.hh"

namespace blocks
{
  namespace events
  {
    struct load_chunk
    {
      cid id;

      load_chunk(const cid &_id) : id(_id) {}
    };
  }
}
