#pragma once

#include <pandaFramework.h>
#include <pandaSystem.h>

#include "common/position.hh"

#include <memory>

namespace blocks {
  class Scene
  {
  public:
    typedef shared_ptr<Scene> ptr;

    Scene(PandaFramework &framework);
    ~Scene();
    Scene() = delete;
    Scene(const Scene &) = delete;

    WindowFramework &window();

    void create_window();
    void init_scene();
    void init_camera();
    void init_lights();
    NodePath make_character(common::wpos &);
    void init_skybox();
    void run();

    NodePath root() { return _scene; }

  protected:
    PandaFramework &_framework;
    WindowFramework *_window;
    NodePath _scene;
    NodePath _camera;
    NodePath _skybox;
    NodePath _sun;
    NodePath _ambient_light;
  };
}
