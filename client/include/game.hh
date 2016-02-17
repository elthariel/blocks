#pragma once

#include "entity.hh"

#include "meshing_task.hh"
#include "meshing_thread.hh"
#include "scene.hh"
#include "TcpConnection.hh"
#include "TcpClient.hh"

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
    void create_player();

    // Panda 3D
    void start();
    AsyncTask::DoneStatus do_task();

    // Network
    void dispatch(TcpConnection<Game, Game>::pointer, uint8_t *);
    void on_initial_pos(TcpConnection<Game, Game>::pointer, fbs::Message*);
    void on_move(fbs::Message*);
    void on_chunk(fbs::Message*);


  protected:
    PandaFramework    _framework;
    MeshingThread     _meshing_thread;

    MeshingTask::ptr  _meshing_task;
    Scene::ptr        _scene;
    TcpClient         _client;

    // Entities and stuff like that
    ex::Entity        _player;

    // Network
    TcpConnection<Game, Game>::pointer _socket = nullptr;
  };
}
