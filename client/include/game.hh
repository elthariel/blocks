#pragma once

#include "TcpClient.hh"
#include "TcpConnection.hh"
#include "entity.hh"
#include "map.hh"
#include "meshing_task.hh"
#include "meshing_thread.hh"
#include "scene.hh"
#include "systems/debug.hh"

#include <asyncTask.h>

namespace blocks
{
  class Game : public ex::EntityX, public AsyncTask
  {
    friend struct systems::Debug;

  public:
    explicit Game(int ac, char **av);
    Game() = delete;
    Game(const Game &) = delete;
    ~Game();

    // Entity system
    void create_systems();
    void update_systems(ex::TimeDelta dt);
    void create_entities();
    void create_player(common::wpos &pos);
    ex::Entity create_character(common::wpos &pos);

    // Getter
    ex::Entity &player() { return _player; }

    // Panda 3D
    void start();
    AsyncTask::DoneStatus do_task();

  protected:
    PandaFramework _framework;

    MeshingTask::ptr _meshing_task;
    Scene::ptr _scene;

    // Entities and stuff like that
    ex::Entity _player;
    Map _map;
  };
}
