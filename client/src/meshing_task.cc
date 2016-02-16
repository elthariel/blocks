
#include "meshing_task.hh"

#include <nodePathCollection.h>
#include <texturePool.h>
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
    if (_mt.mesh_queue_size())
    {
      cout << "Received a mesh" << endl;

      auto result = _mt.mesh_queue_get();
      auto nodepath = _scene.attach_new_node(result.second);
      cpos cp;
      wpos wp(result.first, cp);

      auto subnodes = nodepath.get_children();
      for(auto i = 0; i < subnodes.size(); i++)
      {
        PT(Texture) tex;
        auto node = subnodes[i];
        auto bid = std::stol(node.get_tag("block_id"));

        if (bid == 1)
          tex = TexturePool::load_texture("../media/textures/blocks/dirt.png");
        else if (bid == 2)
          tex = TexturePool::load_texture("../media/textures/blocks/diamond_ore.png");

        tex->set_magfilter(Texture::FilterType::FT_nearest);
        tex->set_minfilter(Texture::FilterType::FT_nearest);
        node.set_texture(tex);
      }
      nodepath.set_pos(wp.x(), wp.y(), wp.z());
    }
    return AsyncTask::DS_cont;
  }
}
