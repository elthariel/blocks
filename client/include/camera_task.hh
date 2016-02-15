#pragma once

#include <asyncTask.h>
#include <nodePath.h>

namespace blocks {
  class CameraTask : public AsyncTask
  {
  public:
    Camera() = delete;
    Camera(NodePath camera);

  protected:
    NodePath _cam;
  };
}
