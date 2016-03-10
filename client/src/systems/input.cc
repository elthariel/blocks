#include "systems/input.hh"

#include "pandaFramework.h"
#include "windowFramework.h"

static void event_callback(const Event *ev, void *data)
{
  auto *handler = reinterpret_cast<blocks::systems::Input *>(data);

  if (handler == nullptr || ev == nullptr)
    return;

  handler->push_incoming_event(*ev);
}

namespace blocks
{
  namespace systems
  {
    Input::Input(PandaFramework &framework, WindowFramework &window)
      : _window(window), _last_pos(0, 0), _diff(0, 0)
    {
      for(auto i = 0; i < _key_down.size(); ++i)
        _key_down[i] = false;

      register_mappings(framework);
    }

    void Input::register_mappings(PandaFramework &framework)
    {
      // Basic movement keys
      _mappings["w"] = events::key::kcode::MOVE_FORWARD;
      _mappings["s"] = events::key::kcode::MOVE_BACKWARD;
      _mappings["a"] = events::key::kcode::MOVE_LEFT;
      _mappings["d"] = events::key::kcode::MOVE_RIGHT;
      _mappings["f"] = events::key::kcode::MOVE_FLY;
      _mappings["space"] = events::key::kcode::MOVE_JUMP;

      // System keys
      _mappings["g"] = events::key::kcode::SYS_GRAB_MOUSE;
      _mappings["p"] = events::key::kcode::SYS_FULLSCREEN;
      _mappings["q"] = events::key::kcode::SYS_EXIT;

      // Debug keys
      _mappings["["] = events::key::kcode::DEBUG_WIREFRAME;
      _mappings["]"] = events::key::kcode::DEBUG_PHYSICS;

      // Tests
      _mappings["e"] = events::key::kcode::MINE;
      _mappings["r"] = events::key::kcode::USE;

      for(auto &kv: _mappings)
      {
        framework.define_key(kv.first, "", event_callback, this);
        framework.define_key(kv.first + "-up", "", event_callback, this);
      }
    }

    void Input::push_incoming_event(const Event &e)
    {
      _incoming_events.push(e);
    }

     void Input::process_key_events(ex::EventManager &event_manager)
    {
      for (auto i = 0; i < _key_down.size(); i++) {
        if (_key_down[i])
        {
          events::key kp(events::key::ktype::REPEAT, events::key::kcode(i));
          event_manager.emit<events::key>(kp);
        }
      }

      while (_incoming_events.size())
      {
        const Event &e = _incoming_events.front();

        std::string name(e.get_name()), type;
        auto dash = name.rfind("-");

        if (dash == std::string::npos)
          type = "down";
        else
        {
          type = name.substr(dash + 1);
          name.erase(dash);
        }

        events::key kp;
        kp.code = _mappings[name];
        if (type == "down")
        {
          kp.type = events::key::ktype::DOWN;
          _key_down[kp.code] = true;
        }
        else
        {
          kp.type = events::key::ktype::UP;
          _key_down[kp.code] = false;
        }

        event_manager.emit<events::key>(kp);

        _incoming_events.pop();
      }
    }

    void Input::process_mouse(ex::EventManager &event_manager)
    {
      auto pointer = _window.get_graphics_window()->get_pointer(0);
      auto gw = _window.get_graphics_window();
      auto w = gw->get_x_size();
      auto h = gw->get_y_size();
      LPoint2f pos(pointer.get_x(), -pointer.get_y());

      _diff = pos - _last_pos;
      _last_pos = pos;

      events::mouse mm;
      mm.size.set_x(w);
      mm.size.set_y(h);
      mm.position = pos;
      mm.offset = _diff;
      event_manager.emit<events::mouse>(mm);
    }

    void Input::configure(ex::EventManager &events)
    {
    }

    void Input::update(ex::EntityManager &entities,
                       ex::EventManager &ev_manager,
                       ex::TimeDelta dt)
    {
      process_key_events(ev_manager);
      process_mouse(ev_manager);
    }
  }
}
