
#include "meshing_task.hh"

#include <iostream>

using namespace std;

namespace blocks
{
  MeshingTask::MeshingTask(MeshingThread &mt, NodePath scene)
    :_mt(mt), _scene(scene)
  {
  }

  AsyncTask::DoneStatus MeshingTask::do_task()
  {
    while (_mt.mesh_queue_size())
    {
      cout << "Received a mesh" << endl;

      auto result = _mt.mesh_queue_get();
      auto nodepath = _scene.attach_new_node(result.second);
      cpos cp;
      wpos wp(result.first, cp);

      nodepath.set_render_mode_wireframe();
      nodepath.set_pos(wp.x(), wp.y(), wp.z());
    }
    return AsyncTask::DS_cont;
  }
}
