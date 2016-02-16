
#include "game.hh"

// Test
#include "seed.hh"
#include "world_generator.hh"
#include "TcpClient.hh"

#include <iostream>

using namespace std;

namespace blocks {
  Game::Game(int ac, char **av)
    : _client(av[1], av[2], this)
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
        _meshing_thread.chunk_queue_put(wg.generate(cid(i, j, k)));

    // mt.chunk_queue_put(wg.generate(cid(1, 0, 0)));
    // mt.chunk_queue_put(wg.generate(cid(0, 1, 0)));

    _scene->run();
  }

  void Game::dispatch(TcpConnection<Game, Game>::pointer socket, uint8_t *buffer)
  {
      std::cout << "Got " <<  buffer << std::endl;
      socket->write((uint8_t*)"Toto tata", 10);
  }
}
