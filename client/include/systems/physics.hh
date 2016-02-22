#pragma once

#include "entity.hh"
#include "events/map.hh"

#include <bulletWorld.h>
#include <memory>

namespace blocks
{
  namespace systems
  {
    class Physics : public ex::System<Physics>, public ex::Receiver<Physics>
    {
    public:
      Physics();
      void configure(ex::EventManager &events);
      void update(ex::EntityManager &entities,
                  ex::EventManager &events,
                  ex::TimeDelta dt);

      void receive(const events::chunk_loaded &e);
    protected:
      shared_ptr<BulletWorld> _world;
    };
  }
}
