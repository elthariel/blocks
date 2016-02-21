#pragma once

#include "common/position.hh"

#include <nodePath.h>

namespace blocks
{
  namespace events
  {
    struct chunk_requested
    {
      common::cid id;

      chunk_requested(const common::cid &_id) : id(_id) {}
    };

    struct chunk_loaded
    {
      common::cid id;
      NodePath  node;

      chunk_loaded(const common::cid &_id, const NodePath &_node)
        : id(_id), node(_node) {}
    };
  }
}
