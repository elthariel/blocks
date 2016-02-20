
#include "systems/chunk_loader.hh"
#include "components/basic.hh"
#include "events/map.hh"
#include "common/position.hh"

#include <iostream>
using namespace std;

namespace blocks
{
  namespace systems
  {
    ChunkLoader::ChunkLoader(Map &map)
      : _map(map)
    {
    }

    void ChunkLoader::configure(ex::EventManager &events)
    {
      events.subscribe<events::server_connected>(*this);
      events.subscribe<events::server_disconnected>(*this);
    }

    static int load = 4;
    static int load_height = 2;
    void ChunkLoader::update(ex::EntityManager &entities,
                             ex::EventManager &events,
                             ex::TimeDelta dt)
    {
      if (!_connected)
        return;

      auto loop = [&](ex::Entity entity,
                     components::Player &player,
                     components::Position &position)
        {
          common::wpos w(position.get_x(), position.get_y(), position.get_z());
          common::cid current_id = w.cid();

          for (auto i = -load; i <= load; ++i)
            for (auto j = -load; j <= load; ++j)
              for (auto k = -load_height; k <= load_height; ++k)
              {
                common::cid iter = common::cid(i, j, k) + current_id;
                if (!(_map.exists(iter) || _map.loading(iter)))
                {
                  events.emit<events::load_chunk>(iter);
                  _map.loading_started(iter);
                }
              }
        };

      entities.each<components::Player, components::Position>(loop);
    }

    void ChunkLoader::receive(const events::server_connected &e)
    {
      _connected = true;
    }

    void ChunkLoader::receive(const events::server_disconnected &e)
    {
      _connected = false;
    }
  }
}
