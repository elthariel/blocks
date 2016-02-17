#pragma once

#include "entity.hh"

#include <mouseWatcher.h>
#include <lpoint2.h>
#include <event.h>

#include <cstdint>
#include <memory>
#include <array>
#include <string>
#include <queue>
#include <map>

class PandaFramework;
class WindowFramework;

namespace blocks {
  namespace systems {

    struct MouseMove
    {
      LPoint2i size;
      LPoint2f position;
      LPoint2f offset;
    };

    struct KeyPress
    {
      enum Type {
        DOWN,
        REPEAT,
        UP
      } type;

      enum Code {
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

      KeyPress() : type(Type::DOWN), code(Code::INVALID) {}
      KeyPress(Type t, Code c) : type(t), code(c) {}
    };

    struct Input : public ex::System<Input>, public nocopy
    {
      Input(PandaFramework &framework, WindowFramework &window);

      void configure(ex::EventManager &events);
      void update(ex::EntityManager &entities,
                  ex::EventManager &events,
                  ex::TimeDelta dt);

      void push_incoming_event(const Event &e);

    protected:
      void register_mappings(PandaFramework &framework);
      void process_key_events(ex::EventManager &event_manager);
      void process_mouse(ex::EventManager &event_manager);

      WindowFramework &_window;
      PT(MouseWatcher) _mouse;

      std::queue<Event> _incoming_events;
      std::array<bool, KeyPress::Code::COUNT> _key_down;
      std::map<std::string, KeyPress::Code> _mappings;
      LPoint2f _last_pos, _diff;
    };
  }
}
