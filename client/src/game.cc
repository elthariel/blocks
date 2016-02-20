
#include "game.hh"

#include "systems/debug.hh"
#include "systems/input.hh"
#include "systems/window_manager.hh"
#include "systems/camera_control.hh"
#include "systems/network.hh"
#include "systems/chunk_loader.hh"

#include "components/basic.hh"

// Test
#include "chunk_generated.h"
#include "TcpClient.hh"
#include "Protocole.hh"

#include <iostream>

using namespace std;

namespace blocks {
  Game::Game(int ac, char **av)
  {
    _framework.open_framework(ac, av);
    _scene = make_shared<Scene>(_framework);

    create_systems();
  }

  void Game::create_systems()
  {
    systems.add<systems::Debug>(*this);
    systems.add<systems::Input>(_framework, _scene->window());
    systems.add<systems::WindowManager>(_scene->window());
    systems.add<systems::CameraControl>();
    systems.add<systems::Network>("127.0.0.1", "3000", this);
    systems.add<systems::ChunkLoader>(_map, _scene->root());

    systems.configure();
  }

  void Game::update_systems(ex::TimeDelta dt)
  {
    systems.update<systems::Debug>(dt);
    systems.update<systems::Input>(dt);
    systems.update<systems::WindowManager>(dt);
    systems.update<systems::CameraControl>(dt);
    systems.update<systems::Network>(dt);
    systems.update<systems::ChunkLoader>(dt);
  }

  void Game::create_entities()
  {
    // create_player();
  }

  void Game::create_player(common::wpos &pos)
  {
    _player = entities.create();
    _player.assign<components::Player>();
    _player.assign<components::Position>(pos.x(), pos.y(), pos.z());
    _player.assign<components::Direction>(0, 1, 0);
    auto camera = _scene->window().get_camera_group();
    camera.set_fluid_pos(LPoint3(pos.x(), pos.y(), pos.z()));
    _player.assign<components::Camera>(camera);
  }

  ex::Entity Game::create_character(common::wpos &pos)
  {
    auto character = entities.create();

    character.assign<components::Character>();
    character.assign<components::Position>(pos.x(), pos.y(), pos.y());
    character.assign<components::Model>(_scene->make_character(pos));

    return character;
  }

  void Game::start()
  {
    cout << "Starting Game !" << endl;

    // create_entities();
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

}
