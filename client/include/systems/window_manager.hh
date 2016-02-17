#pragma once

#include "entity.hh"
#include "systems/input.hh"

#include <mouseWatcher.h>

class WindowFramework;

namespace blocks
{
  namespace systems
  {
    struct WindowManager : public ex::System<WindowManager>,
                           public ex::Receiver<WindowManager>,
                           public nocopy
    {
      WindowManager(WindowFramework &w);

      void update(ex::EntityManager &entities,
                  ex::EventManager &events,
                  ex::TimeDelta dt) {}
      void configure(ex::EventManager &events);
      void receive(const KeyPress &kp);

    protected:
      void toggle_grab_mouse();
      void toggle_fullscreen();
      void recenter_mouse();

      WindowFramework &_window;
      PT(MouseWatcher) _mouse;
    };
  }
}
