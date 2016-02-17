
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
      : _window(window)
    {
      for(auto i = 0; i < _key_down.size(); ++i)
        _key_down[i] = false;

      _mouse = DCAST(MouseWatcher, _window.get_mouse().node());
      register_mappings(framework);
    }

    void Input::register_mappings(PandaFramework &framework)
    {
      // Basic movement keys
      _mappings["w"] = KeyPress::Code::MOVE_FORWARD;
      _mappings["s"] = KeyPress::Code::MOVE_BACKWARD;
      _mappings["a"] = KeyPress::Code::MOVE_LEFT;
      _mappings["d"] = KeyPress::Code::MOVE_RIGHT;
      _mappings["space"] = KeyPress::Code::MOVE_JUMP;

      // System keys
      _mappings["g"] = KeyPress::Code::SYS_GRAB_MOUSE;
      _mappings["f"] = KeyPress::Code::SYS_FULLSCREEN;
      _mappings["q"] = KeyPress::Code::SYS_EXIT;

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
          KeyPress kp(KeyPress::Type::REPEAT, KeyPress::Code(i));
          event_manager.emit<KeyPress>(kp);
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

        KeyPress kp;
        kp.code = _mappings[name];
        if (type == "down")
        {
          kp.type = KeyPress::Type::DOWN;
          _key_down[kp.code] = true;
        }
        else
        {
          kp.type = KeyPress::Type::UP;
          _key_down[kp.code] = false;
        }

        event_manager.emit<KeyPress>(kp);

        _incoming_events.pop();
      }
    }

    void Input::process_mouse(ex::EventManager &event_manager)
    {
      if (_mouse->has_mouse())
      {
        auto gw = _window.get_graphics_window();
        auto w = gw->get_x_size();
        auto h = gw->get_y_size();

        const LPoint2f &pos = _mouse->get_mouse();
        _diff = pos - _last_pos;
        _last_pos = pos;

        MouseMove mm;
        mm.size.set_x(w);
        mm.size.set_y(h);
        mm.position = pos;
        mm.offset = _diff;
        event_manager.emit<MouseMove>(mm);
      }
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
