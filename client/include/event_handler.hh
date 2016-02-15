#pragma once

#include "event.h"

#include "mouseWatcher.h"
#include "asyncTask.h"
#include "lpoint2.h"

#include <cstdint>
#include <memory>
#include <array>
#include <string>
#include <map>

class PandaFramework;
class WindowFramework;

namespace blocks
{
  enum input_event : uint16_t
  {
    INVALID,
    MOVE_FORWARD,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_JUMP,
    SYS_GRAB_MOUSE,
    SYS_EXIT,
    COUNT
  };

  class EventHandler : public AsyncTask
  {
  public:
    typedef shared_ptr<EventHandler> ptr;

    EventHandler() = delete;
    EventHandler(const EventHandler &) = delete;
    EventHandler(PandaFramework &framework, WindowFramework &window);

    void handle_key_event(const Event &e);
    virtual AsyncTask::DoneStatus do_task();
  protected:
    void register_mappings();
    void handle_mouse();
    void handle_camera();
    void handle_keys();

    bool grab_mouse();
    bool ungrab_mouse();
    bool recenter_mouse();

    bool _mouse_grabbed = false;

    PandaFramework &_framework;
    WindowFramework &_window;
    PT(MouseWatcher) _mouse;

    std::array<int32_t, input_event::COUNT> _events;
    std::map<std::string, input_event> _mappings;
    LPoint2f _last_pos, _diff;
  };
}
