
#include "game.hh"

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
    _event_handler = make_shared<EventHandler>(_framework, _scene->window());
    _meshing_task = make_shared<MeshingTask>(_meshing_thread, _scene->root());
  }

  void Game::start()
  {
    cout << "Starting Game !" << endl;

    AsyncTaskManager::get_global_ptr()->add(_meshing_task.get());
    AsyncTaskManager::get_global_ptr()->add(_event_handler.get());

    _scene->run();
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
