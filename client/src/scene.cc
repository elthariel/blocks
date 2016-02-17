
#include "scene.hh"

#include <sceneGraphAnalyzerMeter.h>
#include <antialiasAttrib.h>
#include <directionalLight.h>
#include <ambientLight.h>

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

    auto *meter = new SceneGraphAnalyzerMeter("Meter", _scene.node());
    // meter->setup_window(_window->get_graphics_output());
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
  }

  void Scene::run()
  {
    _framework.main_loop();
    std::cout << "Exiting from main loop" << std::endl;
  }
}
