#pragma once

#include "chunk.hh"

#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <atomic>
#include <utility>
#include "geomNode.h"

namespace blocks {
  class MeshingThread
  {
  public:
    typedef std::pair<cid, PT(GeomNode)> result;

    MeshingThread();
    ~MeshingThread();

    size_t chunk_queue_put(Chunk::ptr);
    size_t chunk_queue_size();

    result mesh_queue_get();
    size_t mesh_queue_size();

    void stop();

  protected:
    size_t mesh_queue_put(result);
    void thread_loop();
    void process_chunk(Chunk::ptr chunk);
    Chunk::ptr wait_for_data();

    std::thread _thread;
    std::atomic_bool _running;
    std::mutex _in_mutex, _out_mutex;
    std::condition_variable _work_signal;
    std::queue<Chunk::ptr> _in_queue;
    std::queue<result> _out_queue;
  };
}
