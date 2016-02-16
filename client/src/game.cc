
#include "game.hh"

// Test
#include "seed.hh"
#include "world_generator.hh"

#include <iostream>

using namespace std;

namespace blocks {
  Game::Game(int ac, char **av)
  {
    _framework.open_framework(ac, av);

    _scene = make_shared<Scene>(_framework);
    _event_handler = make_shared<EventHandler>(_framework, _scene->window());
    _meshing_task = make_shared<MeshingTask>(_meshing_thread, _scene->root());
  }

  void Game::start()
  {
    cout << "Starting Game !" << endl;

    AsyncTaskManager::get_global_ptr()->add(_meshing_task.get());
    AsyncTaskManager::get_global_ptr()->add(_event_handler.get());

    seed s = seed::from_file("/dev/urandom");
    WorldGenerator wg(s);

    auto size = 8;
    for (auto i = 0; i < size; i++)
      for (auto j = 0; j < size; j++)
        for (auto k = -2; k <= 2 ; k++)
          _meshing_thread.input_pipe << wg.generate(cid(i, j, k));

    _scene->run();
  }
}
