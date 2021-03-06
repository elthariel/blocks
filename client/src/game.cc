#include "game.hh"

#include "systems/debug.hh"
#include "systems/input.hh"
#include "systems/window_manager.hh"
#include "systems/camera_control.hh"
#include "systems/network.hh"
#include "systems/chunk_loader.hh"
#include "systems/physics.hh"
#include "components/basic.hh"
#include "session.hh"
#include "textures.hh"

// Test
#include "chunk_generated.h"
#include "TcpClient.hh"
#include "Protocole.hh"

#include <load_prc_file.h>
#include <asyncTask.h>
#include <iostream>

using namespace std;

const std::string prc = R"PRC(
show-frame-rate-meter  #t

# Enable Antialising
framebuffer-multisample 1
multisamples 4

# Enable group collision filter
bullet-filter-algorithm groups-mask
)PRC";

namespace blocks {
  Game::Game(int ac, char **av)
  {
    Session::instance();
    Session::make<Textures>();

    load_prc_file_data("blocks.prc", prc);
    _framework.open_framework(ac, av);
    _scene = make_shared<Scene>(_framework);

    create_entities();
    create_systems();
  }

  Game::~Game()
  {
    Session::instance().destroy();
  }

  void Game::create_systems()
  {
    systems.add<systems::Debug>(*this);
    systems.add<systems::Input>(_framework, _scene->window());
    systems.add<systems::WindowManager>(_scene->window());
    systems.add<systems::CameraControl>();
    systems.add<systems::Network>("127.0.0.1", "3000", this);
    systems.add<systems::ChunkLoader>(_map, _scene->root());
    systems.add<systems::Physics>(_scene);
  }

  void Game::update_systems(ex::TimeDelta dt)
  {
    systems.update<systems::Debug>(dt);
    systems.update<systems::Input>(dt);
    systems.update<systems::WindowManager>(dt);
    systems.update<systems::CameraControl>(dt);
    systems.update<systems::Network>(dt);
    systems.update<systems::ChunkLoader>(dt);
    systems.update<systems::Physics>(dt);
  }

  void Game::create_entities()
  {
    common::wpos pos(0, 0, 30);
    create_player(pos);
  }

  void Game::create_player(common::wpos &pos)
  {
    _player = entities.create();
    _player.assign<components::Player>();

    auto camera = _scene->window().get_camera_group();
    camera.set_fluid_pos(LPoint3(pos.x(), pos.y(), pos.z()));
    _player.assign<components::Node>(camera);
  }

  ex::Entity Game::create_character(common::wpos &pos)
  {
    auto character = entities.create();

    auto node = _scene->make_character(pos);
    node.set_pos(pos.x(), pos.y(), pos.z());

    character.assign<components::Node>(node);
    character.assign<components::Model>();

    return character;
  }

  void Game::start()
  {
    cout << "Starting Game !" << endl;

    AsyncTaskManager::get_global_ptr()->add(this);

    systems.configure();
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
