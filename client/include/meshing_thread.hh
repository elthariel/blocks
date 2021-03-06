#pragma once

#include "common/chunk.hh"
#include "pipe.hh"

#include <geomNode.h>
#include <bulletShape.h>
#include <bulletRigidBodyNode.h>

#include <thread>
#include <atomic>
#include <utility>
#include <memory>
#include <tuple>

namespace blocks {

  class MeshingThread
  {
  public:
    typedef std::tuple<common::cid,
                       PT(GeomNode),
                       PT(BulletRigidBodyNode),
                       common::Chunk::ptr> result;
    typedef shared_ptr<MeshingThread> ptr;

    MeshingThread(Pipe<common::Chunk::ptr> &_input,
                  Pipe<MeshingThread::result> &_output);
    ~MeshingThread();

    void stop();

  protected:
    void thread_loop();
    void process_chunk(common::Chunk::ptr chunk);

    std::atomic_bool _running;
    std::thread _thread;

    Pipe<common::Chunk::ptr> &_input_pipe;
    Pipe<MeshingThread::result> &_output_pipe;
  };
}
