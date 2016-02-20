#pragma once

#include "common/chunk.hh"
#include "pipe.hh"

#include <thread>
#include <atomic>
#include <utility>
#include <memory>
#include "geomNode.h"

namespace blocks {
  class MeshingThread
  {
  public:
    typedef std::pair<common::cid, PT(GeomNode)> result;
    typedef shared_ptr<MeshingThread> ptr;

    MeshingThread(Pipe<common::Chunk::ptr> &_input,
                  Pipe<MeshingThread::result> &_output);
    ~MeshingThread();

    void stop();

  protected:
    void thread_loop();
    void process_chunk(common::Chunk::ptr chunk);

    std::thread _thread;
    std::atomic_bool _running;

    Pipe<common::Chunk::ptr> &_input_pipe;
    Pipe<MeshingThread::result> &_output_pipe;
  };
}
