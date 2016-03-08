#pragma once

#include <bulletRigidBodyNode.h>

namespace blocks
{
  namespace events
  {
    struct bullet_attach
    {
      bullet_attach(PT(BulletRigidBodyNode) o) : body(o) {}
      PT(BulletRigidBodyNode) body;
    };

    struct bullet_remove : public bullet_attach
    {
      using bullet_attach::bullet_attach;
    };
  }
}
