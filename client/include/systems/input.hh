#pragma once

#include "entity.hh"
#include "events/input.hh"

#include <mouseWatcher.h>
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

      std::queue<Event> _incoming_events;
      std::array<bool, events::key::kcode::COUNT> _key_down;
      std::map<std::string, events::key::kcode> _mappings;
      LPoint2f _last_pos, _diff;
    };
  }
}
