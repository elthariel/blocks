#pragma once

#include "common/position.hh"
#include "entity.hh"

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
      ex::Entity entity;

      chunk_loaded(const ex::Entity &_entity)
        : entity(_entity) {}
    };
  }
}
