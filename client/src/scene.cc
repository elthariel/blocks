
#include "scene.hh"

#include <sceneGraphAnalyzerMeter.h>
#include <antialiasAttrib.h>

#include <iostream>

using namespace std;

namespace blocks
{
  Scene::Scene(PandaFramework &framework)
    :_framework(framework)
  {
    create_window();
    init_scene();
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

    _scene = _window->get_render();
    // _scene.set_antialias(AntialiasAttrib::M_auto);

    _window->set_background_type(WindowFramework::BackgroundType::BT_black);

    auto *meter = new SceneGraphAnalyzerMeter("Meter", _scene.node());
    meter->setup_window(_window->get_graphics_output());

    _window->enable_keyboard();
  }

  void Scene::init_scene()
  {
  }

  void Scene::run()
  {
    _framework.main_loop();
  }
}
