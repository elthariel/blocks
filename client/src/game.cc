
#include "game.hh"

#include "systems/debug.hh"
#include "systems/input.hh"
#include "systems/window_manager.hh"
#include "systems/camera_control.hh"

#include "components/basic.hh"

// Test
#include "seed.hh"
#include "world_generator.hh"
#include "chunk_generated.h"
#include "TcpClient.hh"

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

    seed s = seed::from_file("/dev/urandom");
    WorldGenerator wg(s);

    auto size = 24;
    for (auto i = 0; i < size; i++)
      for (auto j = 0; j < size; j++)
        for (auto k = -2; k <= 2 ; k++)
          _meshing_thread.input_pipe << wg.generate(cid(i, j, k));

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
      auto atype = message->body();
      auto player = static_cast<const fbs::Player*>(message->body());
      std::cout << "Got " << player->pos()->x() << std::endl;
  }
}
