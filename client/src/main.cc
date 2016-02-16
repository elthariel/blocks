// #include "pandaFramework.h"
// #include "pandaSystem.h"
// #include "meta.hh"
// #include "multi_array.hh"

#include <iostream>
#include <string>
#include		   <boost/asio.hpp>
#include		   <boost/thread.hpp>
using namespace std;

#include "Client.hh"


int			   main(int ac, char **av)
{
  if (ac != 3)
  {
    std::cerr << "Usage : ./client IP PORT" << std::endl;
    return (0);
  }
  Client client(av[1], av[2]);
  return (0);
}

// int main(int ac, char **av) {
//   cout << Power<4, 2>::value << endl;
// }

// int main(int argc, char *argv[]) {
//   // Open a new window framework
//   PandaFramework framework;
//   framework.open_framework(argc, argv);
//   //set the window title to My Panda3D Window
//   framework.set_window_title("Blocks again !");
//
//   //open the window
//   WindowFramework *window = framework.open_window();
//
//   window->setup_trackball();
//
//   // Load the environment model.
//   NodePath scene = window->load_model(framework.get_models(), "environment");
//   // Reparent the model to render.
//   scene.reparent_to(window->get_render());
//   // Apply scale and position transforms to the model.
//   scene.set_scale(0.25f, 0.25f, 0.25f);
//   scene.set_pos(-8, 42, 0);
//
//   //do the main loop, equal to run() in python
//   framework.main_loop();
//   //close the window framework
//   framework.close_framework();
//   return (0);
// }
