
#include "event_handler.hh"

#include <pandaFramework.h>


#include <iostream>

namespace blocks
{
  EventHandler::EventHandler(PandaFramework &framework, WindowFramework &window)
    : _framework(framework), _window(window)
  {
    for(auto i = 0; i < _events.size(); ++i)
      _events[i] = 0;

    _mouse = DCAST(MouseWatcher, _window.get_mouse().node());
    register_mappings();

  }

  static void event_callback(const Event *ev, void *data)
  {
    auto *handler = reinterpret_cast<EventHandler *>(data);

    if (handler == nullptr)
    {
    //   std::cout << "Event called on null handler" << std::endl;
      return;
    }

    if (ev == nullptr)
    {
    //   std::cout << "Handler received a null event" << std::endl;
      return;
    }

    handler->handle_key_event(*ev);
  }

  void EventHandler::register_mappings()
  {
    // Basic movement keys
    _mappings["w"] = input_event::MOVE_FORWARD;
    _mappings["s"] = input_event::MOVE_BACKWARD;
    _mappings["a"] = input_event::MOVE_LEFT;
    _mappings["d"] = input_event::MOVE_RIGHT;
    _mappings["space"] = input_event::MOVE_JUMP;

    // System keys
    _mappings["g"] = input_event::SYS_GRAB_MOUSE;
    _mappings["q"] = input_event::SYS_EXIT;

    for(auto &kv: _mappings)
    {
      _framework.define_key(kv.first, "", event_callback, this);
      _framework.define_key(kv.first + "-up", "", event_callback, this);
    }
  }

  void EventHandler::handle_key_event(const Event &e)
  {
    auto name(e.get_name());
    auto pos_up = name.rfind("-up");
    int16_t down = 0;

    if (pos_up != std::string::npos)
      name.erase(pos_up);
    else
      down = 1;

    std::cout << "Received event: " << name << " down: " << down << std::endl;

    input_event et = _mappings[name];
    _events[et] = down;
  }

  bool EventHandler::recenter_mouse()
  {
    int w, h;

    if(!_mouse->has_mouse())
      return false;

    auto gw = _window.get_graphics_window();
    w = gw->get_x_size();
    h = gw->get_y_size();

    return gw->move_pointer(0, w/2, h/2);
  }

  bool EventHandler::grab_mouse()
  {

    if(!recenter_mouse())
      return false;

    auto props = _window.get_graphics_window()->get_properties();
    if (_mouse_grabbed)
    {
      // Ungrab the mouse
      props.set_cursor_hidden(false);
      props.set_mouse_mode(WindowProperties::M_absolute);
    }
    else
    {
      props.set_cursor_hidden(true);
      props.set_mouse_mode(WindowProperties::M_relative);
    }

    _mouse_grabbed = !_mouse_grabbed;
    _window.get_graphics_window()->request_properties(props);
    return true;
  }

  void EventHandler::handle_mouse()
  {
    if (_mouse->has_mouse() && _mouse_grabbed)
    {
      const LPoint2f &pos = _mouse->get_mouse();
      _diff = pos - _last_pos;
      _last_pos = pos;

      // std::cout << "Mouse: " << diff.get_x() << ":" << diff.get_y() << std::endl;
    }
  }

  void EventHandler::handle_keys()
  {
    if (_events[input_event::SYS_EXIT])
      exit(0);

    if (_events[input_event::SYS_GRAB_MOUSE])
    {
      grab_mouse();
      _events[input_event::SYS_GRAB_MOUSE] = 0;
    }


  }

  const float cam_sensitivity = 50.0;
  const float move_speed = 0.5;
  void EventHandler::handle_camera()
  {
    NodePath cam = _window.get_camera_group();
    LVecBase3 hpr = cam.get_hpr();

    hpr.add_x(_diff.get_x() * cam_sensitivity * -1.0);
    hpr.add_y(_diff.get_y() * cam_sensitivity);

    cam.set_hpr(hpr);

    LPoint3 pos = cam.get_pos();
    LPoint3 offset(0);
    auto forward = cam.get_quat().get_forward();
    auto right = cam.get_quat().get_right();

    offset += forward * move_speed * _events[input_event::MOVE_FORWARD];
    offset += forward * move_speed * _events[input_event::MOVE_BACKWARD] * -1;;
    offset += right * move_speed * _events[input_event::MOVE_RIGHT];
    offset += right * move_speed * _events[input_event::MOVE_LEFT] * -1;;
    offset.add_z(move_speed * _events[input_event::MOVE_JUMP]);

    cam.set_fluid_pos(pos + offset);
  }

  AsyncTask::DoneStatus EventHandler::do_task()
  {
    handle_mouse();
    handle_keys();
    handle_camera();
    return AsyncTask::DS_cont;
  }
}
