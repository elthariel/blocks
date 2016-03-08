#pragma once

#include "common/util.hh"
#include "common/position.hh"

#include <pandaFramework.h>
#include <pandaSystem.h>

#include <memory>

namespace blocks {
  class Scene : public nocopy
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
    void init_skybox();
    void init_aim();
    NodePath make_character(common::wpos &);
    void run();

    NodePath root() { return _scene; }
    NodePath aim_cube() { return _aim_cube; }

  protected:
    PandaFramework &_framework;
    WindowFramework *_window;
    NodePath _scene;
    NodePath _camera;
    NodePath _skybox;
    NodePath _sun;
    NodePath _ambient_light;
    NodePath _aim_pointer;
    NodePath _aim_cube;
  };
}
