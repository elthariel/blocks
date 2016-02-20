
#include "scene.hh"
#include "models/make.hh"
#include "models/box.hh"

#include <sceneGraphAnalyzerMeter.h>
#include <antialiasAttrib.h>
#include <directionalLight.h>
#include <ambientLight.h>
#include <texturePool.h>

#include <cstdlib>
#include <iostream>

using namespace std;

namespace blocks
{
  Scene::Scene(PandaFramework &framework)
    :_framework(framework)
  {
    create_window();
    init_scene();
    init_lights();
    init_skybox();
    _scene.ls();
  }

  Scene::~Scene()
  {
    _framework.close_framework();
  }

  WindowFramework &Scene::window()
  {
    return *_window;
  }

  void Scene::create_window()
  {
    _framework.set_window_title("Blocks again, bitch !");
    _window = _framework.open_window();
    if (_window == nullptr)
    {
      cout << "Unable to open a window" << endl;
      exit(1);
    }
    _window->enable_keyboard();
  }

  void Scene::init_scene()
  {
    _scene = _window->get_render();
    _scene.set_antialias(AntialiasAttrib::M_auto);
    _window->set_background_type(WindowFramework::BackgroundType::BT_black);

    _camera = window().get_camera_group();
    _camera.set_pos(0, 0, 0);
    _camera.set_hpr(0, 0, 0);
    _camera.heads_up(0, 10, 1);

    if (getenv("GRAPH_METER"))
    {
      auto *meter = new SceneGraphAnalyzerMeter("Meter", _scene.node());
      meter->setup_window(_window->get_graphics_output());
    }
  }

  void Scene::init_lights()
  {

    PT(DirectionalLight) sun = new DirectionalLight("light:sun");
    sun->set_direction(LVector3f(-0.5, 0, -1));
    _sun = _scene.attach_new_node(sun);
    _sun.set_pos(1000, 0, 1000);

    PT(AmbientLight) ambient = new AmbientLight("light:ambient");
    LVecBase4f color(0.7, 0.7, 0.7, 1);
    ambient->set_color(color);
    _ambient_light = _scene.attach_new_node(ambient);


    _scene.set_light(_sun);
    _scene.set_light(_ambient_light);

    auto cube = _scene.attach_new_node(models::make<models::Box>("cube", 1, 2, 3));
    cube.set_render_mode_wireframe();
  }

  void Scene::init_skybox()
  {
    _skybox = _camera.attach_new_node(models::make<models::Box>("skybox", 10000));
    _skybox.set_compass();
    _skybox.set_two_sided(true);
    _skybox.set_depth_write(false);
    _skybox.set_depth_test(false);
    _skybox.set_bin("background", 1);
    _skybox.set_light_off();
    _skybox.set_pos(-5000, -5000, -5000);
    //_skybox.set_tex_gen(TextureStage::get_default(), TexGenAttrib::M_world_cube_map);
    _skybox.set_tex_gen(TextureStage::get_default(), TexGenAttrib::M_world_position);

    auto tex = TexturePool::load_cube_map("../media/textures/skybox_#.png");
    _skybox.set_texture(tex);
  }

  void Scene::run()
  {
    _framework.main_loop();
    std::cout << "Exiting from main loop" << std::endl;
  }
}
