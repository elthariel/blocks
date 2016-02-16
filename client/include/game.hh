#pragma once

//#include "entity.hh"

#include "event_handler.hh"
#include "meshing_task.hh"
#include "meshing_thread.hh"
#include "scene.hh"

namespace blocks
{
  class Game
  {
  public:
    Game(int ac, char **av);
    Game() = delete;
    Game(const Game &) = delete;

    void start();
  protected:
    PandaFramework    _framework;
    MeshingThread     _meshing_thread;

    MeshingTask::ptr  _meshing_task;
    EventHandler::ptr _event_handler;
    Scene::ptr        _scene;
  };
}
