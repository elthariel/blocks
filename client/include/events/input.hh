#pragma once

#include <lpoint2.h>

namespace blocks
{
  namespace events
  {
    struct mouse
    {
      LPoint2i size;
      LPoint2f position;
      LPoint2f offset;
    };

    struct key
    {
      enum ktype {
        DOWN,
        REPEAT,
        UP
      } type;

      enum kcode {
        INVALID,
        MOVE_FORWARD,
        MOVE_BACKWARD,
        MOVE_LEFT,
        MOVE_RIGHT,
        MOVE_JUMP,
        SYS_GRAB_MOUSE,
        SYS_FULLSCREEN,
        SYS_EXIT,
        COUNT
      } code;

      key() : type(ktype::DOWN), code(kcode::INVALID) {}
      key(ktype t, kcode c) : type(t), code(c) {}
    };
  }
}
