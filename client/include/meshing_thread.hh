#pragma once

#include "chunk.hh"
#include "pipe.hh"

#include <thread>
#include <utility>
#include "geomNode.h"

namespace blocks {
  class MeshingThread
  {
  public:
    typedef std::pair<cid, PT(GeomNode)> result;

    MeshingThread();
    ~MeshingThread();

    void stop();

    Pipe<Chunk::ptr> input_pipe;
    Pipe<result> output_pipe;
  protected:
    void thread_loop();
    void process_chunk(Chunk::ptr chunk);

    std::thread _thread;
    std::atomic_bool _running;
  };
}
