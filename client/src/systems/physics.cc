
#include <systems/physics.hh>

namespace blocks
{
  namespace systems
  {
    Physics::Physics()
      : _world(new BulletWorld())
    {
      _world->set_gravity(0, 0, -9.81f);
    }
  }
}
