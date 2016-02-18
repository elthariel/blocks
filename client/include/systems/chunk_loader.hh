#pragma once

#include "map.hh"
#include "events/network.hh"

namespace blocks
{
  namespace systems
  {
    class ChunkLoader : public ex::System<ChunkLoader>,
                        public ex::Receiver<ChunkLoader>,
                        public nocopy
    {
    public:
      ChunkLoader(Map &map);
      void configure(ex::EventManager &events);
      void update(ex::EntityManager &entities,
                  ex::EventManager &events,
                  ex::TimeDelta dt);

      void receive(const events::server_connected &e);
      void receive(const events::server_disconnected &e);

    protected:
      Map &_map;
      
      bool _connected = false;
    };
  }
}
