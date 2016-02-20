#pragma once

#include "entity.hh"
#include "common/position.hh"

#include <lpoint3.h>
#include <lvector3.h>
#include <nodePath.h>

namespace blocks
{
  namespace components
  {
    struct Position : public LPoint3f
    {
      using LPoint3f::LPoint3f;
      Position(const LVecBase3f &other) : LPoint3f(other) {}
      common::wpos to_wpos()
      {
        common::wpos res(get_x(), get_y(), get_z());
        return res;
      }
    };

    struct Direction : public LVector3f
    {
      using LVector3f::LVector3f;
      Direction(const LVecBase3f &other) : LVector3f(other) {}
    };

    struct Camera : public NodePath
    {
      Camera(const NodePath &n): NodePath(n) {}
      using NodePath::NodePath;
    };

    struct Character
    {
    };

    struct Model : public NodePath
    {
      Model(const NodePath &n): NodePath(n) {}
      using NodePath::NodePath;
    };

    struct Player
    {
    };

  }
}