#pragma once

#include "entity.hh"

#include <bulletWorld.h>
#include <memory>

namespace blocks
{
  namespace systems
  {
    class Physics
    {
    public:
      Physics();

    protected:
      shared_ptr<BulletWorld> _world;
    };
  }
}
