
#include "systems/window_manager.hh"

#include <windowFramework.h>

namespace blocks
{
  namespace systems
  {
    WindowManager::WindowManager(WindowFramework &w)
      : _window(w)
    {
      _mouse = DCAST(MouseWatcher, _window.get_mouse().node());
    }

    void WindowManager::configure(ex::EventManager &events)
    {
      events.subscribe<KeyPress>(*this);
    }

    void WindowManager::receive(const KeyPress &kp)
    {
      if (kp.type == KeyPress::Type::DOWN)
      {
        if (kp.code == KeyPress::Code::SYS_GRAB_MOUSE)
          toggle_grab_mouse();
        else if (kp.code == KeyPress::Code::SYS_FULLSCREEN)
          toggle_fullscreen();
        else if (kp.code == KeyPress::Code::SYS_EXIT)
          exit(0);
      }
    }

    void WindowManager::recenter_mouse()
    {
      int w, h;

      if(!_mouse->has_mouse())
        return;

      auto gw = _window.get_graphics_window();
      w = gw->get_x_size();
      h = gw->get_y_size();

      gw->move_pointer(0, w/2, h/2);
    }

    void WindowManager::toggle_grab_mouse()
    {
      recenter_mouse();

      auto props = _window.get_graphics_window()->get_properties();
      if (props.get_mouse_mode() == WindowProperties::M_absolute)
      {
        // Grab the mouse
        props.set_cursor_hidden(true);
        props.set_mouse_mode(WindowProperties::M_relative);
      }
      else
      {
        // Ungrab the mouse
        props.set_cursor_hidden(false);
        props.set_mouse_mode(WindowProperties::M_absolute);
      }

      _window.get_graphics_window()->request_properties(props);
    }

    void WindowManager::toggle_fullscreen()
    {
      auto props = _window.get_graphics_window()->get_properties();

      if (props.get_fullscreen())
      {
        props.set_fullscreen(false);
      }
      else
      {
        props.set_fullscreen(true);
      }

      _window.get_graphics_window()->request_properties(props);
    }
  }
}
