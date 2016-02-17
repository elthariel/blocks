#pragma once

#include "systems/input.hh"
#include "components/basic.hh"

namespace blocks
{
  namespace systems
  {
    struct CameraControl : public ex::System<CameraControl>,
                           public ex::Receiver<CameraControl>
    {
      void configure(ex::EventManager &events);
      void update(ex::EntityManager &entities,
                  ex::EventManager &events,
                  ex::TimeDelta dt);

      void receive(const KeyPress &e);
      void receive(const MouseMove &e);
    protected:
      LPoint2f  mouse_offset;
      LVector3f move;
      LVector3f move_absolute;
    };
  }
}
