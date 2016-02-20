#pragma once

#include "map.hh"
#include "events/network.hh"
#include "meshing_thread.hh"

#include <nodePath.h>

namespace blocks
{
  namespace systems
  {
    class ChunkLoader : public ex::System<ChunkLoader>,
                        public ex::Receiver<ChunkLoader>,
                        public nocopy
    {
    public:
      ChunkLoader(Map &map, NodePath scene);
      void configure(ex::EventManager &events);
      void update(ex::EntityManager &entities,
                  ex::EventManager &events,
                  ex::TimeDelta dt);

      void fetch_meshed_chunks();

      void receive(const events::server_connected &e);
      void receive(const events::server_disconnected &e);
      void receive(const events::chunk_received &e);

    protected:
      Map &_map;
      NodePath _scene;

      bool _connected = false;

      Pipe<common::Chunk::ptr> _pipe_to_mesher;
      Pipe<MeshingThread::result> _pipe_from_mesher;
      std::vector<MeshingThread::ptr> _meshing_threads;
    };
  }
}
