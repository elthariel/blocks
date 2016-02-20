#pragma once

#include "entity.hh"

#include "meshing_task.hh"
#include "meshing_thread.hh"
#include "scene.hh"
#include "TcpConnection.hh"
#include "TcpClient.hh"
#include "map.hh"
#include "components/basic.hh"

#include <asyncTask.h>

namespace blocks
{
  class Game : public ex::EntityX, public AsyncTask
  {
  public:
    explicit Game(int ac, char **av);
    Game() = delete;
    Game(const Game &) = delete;

    // Entity system
    void create_systems();
    void update_systems(ex::TimeDelta dt);
    void create_entities();
    void create_player(common::wpos &pos);
    ex::Entity create_character(common::wpos &pos);


    // Panda 3D
    void start();
    AsyncTask::DoneStatus do_task();

  protected:
    PandaFramework    _framework;
    MeshingThread     _meshing_thread;

    MeshingTask::ptr  _meshing_task;
    Scene::ptr        _scene;

    // Entities and stuff like that
    ex::Entity        _player;
    Map               _map;
  };
}
