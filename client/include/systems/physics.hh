#pragma once

#include "entity.hh"
#include "events/map.hh"
#include "events/input.hh"
#include "events/physics.hh"
#include "scene.hh"

#include <bulletWorld.h>
#include <memory>

namespace blocks
{
  namespace systems
  {
    class Physics : public ex::System<Physics>, public ex::Receiver<Physics>
    {
    public:
      Physics(Scene::ptr scene);
      void configure(ex::EntityManager &entities, ex::EventManager &events);
      void update(ex::EntityManager &entities,
                  ex::EventManager &events,
                  ex::TimeDelta dt);

      void receive(const events::bullet_attach &e);
      void receive(const events::bullet_remove &e);
      void receive(const events::key &e);
    protected:
      PT(BulletWorld) _world;
      Scene::ptr              _scene;

      NodePath                _debug;
      bool                    _debug_loaded = false;
    };
  }
}
