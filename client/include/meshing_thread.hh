#pragma once

#include "chunk.hh"

#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include "geomNode.h"

namespace blocks {
  class MeshingThread {
  public:
    MeshingThread();

    size_t chunk_queue_put(Chunk::ptr);
    size_t chunk_queue_size();

    size_t mesh_queue_put(PT(GeomNode));
    PT(GeomNode) mesh_queue_get();
    size_t mesh_queue_size();

    void stop();

  protected:
    void thread_loop();

    std::thread _thread;
    std::atomic_bool _running;
    std::mutex _in_mutex, _out_mutex;
    std::condition_variable _work_signal;
    std::queue<Chunk::ptr> _in_queue;
    std::queue<PT(GeomNode)> _out_queue;
  };
}
