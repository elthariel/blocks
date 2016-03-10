#pragma once

#include "common/thread.hh"
#include "common/chunk.hh"
#include "pipe.hh"

#include <geomNode.h>
#include <bulletShape.h>
#include <bulletRigidBodyNode.h>

#include <utility>
#include <memory>
#include <tuple>

namespace blocks {

  class MeshingThread : public Thread
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

  protected:
    void main_loop();
    void process_chunk(common::Chunk::ptr chunk);

    Pipe<common::Chunk::ptr> &_input_pipe;
    Pipe<MeshingThread::result> &_output_pipe;
  };
}
