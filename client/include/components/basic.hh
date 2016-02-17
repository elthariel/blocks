#pragma once

#include "entity.hh"

#include <lpoint3.h>
#include <lvector3.h>
#include <pandaNode.h>

namespace blocks
{
  namespace components
  {
    struct Position : public LPoint3f
    {
      using LPoint3f::LPoint3f;
    };

    struct Direction : public LVector3f
    {
      using LVector3f::LVector3f;
    };

    struct Camera : public NodePath
    {
      Camera(const NodePath &n): NodePath(n) {}
      using NodePath::NodePath;
    };

    struct Player
    {
    };

  }
}
