
#include "game.hh"

#include "systems/debug.hh"
#include "systems/input.hh"
#include "systems/window_manager.hh"
#include "systems/camera_control.hh"

#include "components/basic.hh"

// Test
#include "chunk_generated.h"
#include "TcpClient.hh"
#include "Protocole.hh"

#include <iostream>

using namespace std;

namespace blocks {
  Game::Game(int ac, char **av)
    : _client(av[1], av[2], this)
  {
    _framework.open_framework(ac, av);
    _scene = make_shared<Scene>(_framework);
    _meshing_task = make_shared<MeshingTask>(_meshing_thread, _scene->root());

    create_systems();
  }

  void Game::create_systems()
  {
    systems.add<systems::Debug>();
    systems.add<systems::Input>(_framework, _scene->window());
    systems.add<systems::WindowManager>(_scene->window());
    systems.add<systems::CameraControl>();

    systems.configure();
  }

  void Game::update_systems(ex::TimeDelta dt)
  {
    systems.update<systems::Debug>(dt);
    systems.update<systems::Input>(dt);
    systems.update<systems::WindowManager>(dt);
    systems.update<systems::CameraControl>(dt);
  }

  void Game::create_entities()
  {
    create_player();
  }

  void Game::create_player()
  {
    _player = entities.create();
    _player.assign<components::Player>();
    _player.assign<components::Position>(0, 0, 0);
    _player.assign<components::Direction>(0, 1, 0);
    _player.assign<components::Camera>(_scene->window().get_camera_group());
  }

  void Game::start()
  {
    cout << "Starting Game !" << endl;

    create_entities();

    AsyncTaskManager::get_global_ptr()->add(_meshing_task.get());
    AsyncTaskManager::get_global_ptr()->add(this);

    _scene->run();
  }

  AsyncTask::DoneStatus Game::do_task()
  {
    double dt = get_dt();
    update_systems(dt);

    // Can never return something else or deadlock
    return AsyncTask::DS_cont;
  }

  void Game::dispatch(TcpConnection<Game, Game>::pointer socket, uint8_t *buffer)
  {
      auto message = flatbuffers::GetMutableRoot<fbs::Message>(buffer);
      switch(message->action())
      {
          case fbs::Action::Action_INITIAL_POS  : on_initial_pos(socket, message); break;
          case fbs::Action::Action_MOVE         : break;
          case fbs::Action::Action_ASK_CHUNK    : break;
          case fbs::Action::Action_CHUNK        : on_chunk(message); break;
          case fbs::Action::Action_NEW_BLOCK     : break;
          case fbs::Action::Action_DELETE_BLOCK  : break;

      }

  }

  void Game::on_initial_pos(TcpConnection<Game, Game>::pointer socket, fbs::Message *message)
  {
      if (_socket == nullptr)
        _socket = socket;

      auto player = static_cast<const fbs::Player*>(message->body());
      auto size = 8;
      for (auto i = 0; i < size; i++)
        for (auto j = 0; j < size; j++)
          for (auto k = -2; k <= 2 ; k++)
          {
            auto pos = cid(i, j, k);
            _socket->write(Protocole::create_message(fbs::Action::Action_ASK_CHUNK,
                                                       fbs::AType::AType_PosObj, &pos));
          }
  }


  void Game::on_chunk(fbs::Message *message)
  {
    auto chunk = static_cast<const fbs::Chunk*>(message->body());
    auto test = Chunk::deserialize(chunk);
    _meshing_thread.input_pipe << test;

  }
}
