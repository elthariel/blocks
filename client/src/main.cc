
#include "game.hh"

#include <pandaFramework.h>
#include <pandaSystem.h>
#include <iostream>
#include <string>

using namespace std;
using namespace blocks;


int main(int ac, char **av)
{
  Game game(ac, av);

  game.start();
  return 0;
}

//   // Open a new window framework
//   PandaFramework framework;
//   framework.open_framework(argc, argv);
//   //set the window title to My Panda3D Window
//   framework.set_window_title("Blocks again !");

//   //open the window
//   WindowFramework *window = framework.open_window();

//   window->setup_trackball();

//   seed s = seed::from_file("/dev/urandom");
//   WorldGenerator wg(s);
//   MeshingThread mt;

//   mt.chunk_queue_put(wg.generate(cid(0, 0, 0)));
//   mt.chunk_queue_put(wg.generate(cid(1, 0, 0)));
//   mt.chunk_queue_put(wg.generate(cid(0, 1, 0)));

//   MeshingTask task(mt, window->get_render());
//   AsyncTaskManager::get_global_ptr()->add(&task);

//   //do the main loop, equal to run() in python
//   framework.main_loop();
//   //close the window framework
//   framework.close_framework();
//   return (0);
// }
