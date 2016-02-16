#pragma once

#include "meshing_thread.hh"

#include <asyncTask.h>
#include <nodePath.h>

#include <memory>

namespace blocks {
  class MeshingTask : public AsyncTask
  {
  public:
    typedef std::shared_ptr<MeshingTask> ptr;

    MeshingTask(MeshingThread &mt, NodePath scene);

    virtual AsyncTask::DoneStatus do_task();
  protected:
    MeshingThread &_mt;
    NodePath _scene;
  };
}
