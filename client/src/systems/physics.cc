
#include <systems/physics.hh>

#include <iostream>

namespace blocks
{
  namespace systems
  {
    Physics::Physics()
      : _world(new BulletWorld())
    {
      _world->set_gravity(0, 0, -9.81f);
    }

    void Physics::configure(ex::EventManager &events)
    {
      events.subscribe<events::chunk_loaded>(*this);
    }

    void Physics::update(ex::EntityManager &entities,
                         ex::EventManager &events,
                         ex::TimeDelta dt)
    {
    }

    void Physics::receive(const events::chunk_loaded &e)
    {
      // std::cout << "Loaded chunk: " << e.entity << std::endl;
    }
  }
}
