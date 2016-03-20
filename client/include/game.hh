#pragma once

#include "TcpClient.hh"
#include "TcpConnection.hh"
#include "entity.hh"
#include "map.hh"
#include "meshing_task.hh"
#include "meshing_thread.hh"
#include "scene.hh"
#include "systems/debug.hh"
#include "session.hh"

#include <asyncTask.h>

// Declares the main function, so we can befriend it in Game
// Only allowing the main function to initialize the game.
int main(int, char **);

namespace blocks
{
  class Game;
  struct AsyncTaskAdapter: public AsyncTask
  {
    AsyncTaskAdapter(Game &g);
    AsyncTask::DoneStatus do_task();
    Game &_game;
  };

  class Game : public ex::EntityX, public Session<Game>
  {
    friend systems::Debug;
    friend int ::main(int, char **);

  public:
    Game() = delete;
    ~Game();

    // Getter
    ex::Entity &player() { return _player; }

    // Panda 3D
    void start();
    void tick(double dt);

    // XXX: This should be in a system, or at least not public
    ex::Entity create_character(common::wpos &pos);

    template <class T>
    static std::shared_ptr<T> system()
    {
      return instance().systems.system<T>();
    }

  protected:
    explicit Game(int ac, char **av);

    // Entity system
    void create_systems();
    void update_systems(ex::TimeDelta dt);
    void create_entities();
    void create_player(common::wpos &pos);

    // Various services
    void create_services();

    PandaFramework _framework;
    AsyncTaskAdapter _async;

    MeshingTask::ptr _meshing_task;
    Scene::ptr _scene;

    // Entities and stuff like that
    ex::Entity _player;
    Map _map;
  };
}
