#pragma once

#include "events/input.hh"
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

      void receive(const events::key &e);
      void receive(const events::mouse &e);
    protected:
      bool _do_fly = false;
      bool _do_jump = false;
      LPoint2f  _mouse_offset;
      LVector3f _move;
      LVector3f _move_absolute;
    };
  }
}
