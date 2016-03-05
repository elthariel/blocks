
#include "systems/chunk_loader.hh"
#include "components/basic.hh"
#include "common/position.hh"
#include "common/chunk.hh"
#include "events/map.hh"
#include "events/physics.hh"

#include <nodePathCollection.h>
#include <texturePool.h>
#include <memory>
#include <iostream>

using namespace std;

namespace blocks
{
  namespace systems
  {
    ChunkLoader::ChunkLoader(Map &map, NodePath scene)
      : _map(map), _scene(scene)
    {
      auto cores = std::thread::hardware_concurrency();
      auto threads = cores - 2;
      if (threads < 1)
        threads = 1;

      for (auto i = 0; i < threads; i++)
        _meshing_threads.push_back(
          std::make_shared<MeshingThread>(_pipe_to_mesher, _pipe_from_mesher)
        );
    }

    void ChunkLoader::configure(ex::EventManager &events)
    {
      events.subscribe<events::server_connected>(*this);
      events.subscribe<events::server_disconnected>(*this);
      events.subscribe<events::chunk_received>(*this);
      events.subscribe<events::block_update>(*this);

    }

    static int load = 3;
    static int load_height = 2;
    void ChunkLoader::update(ex::EntityManager &entities,
                             ex::EventManager &events,
                             ex::TimeDelta dt)
    {
      if (!_connected)
        return;

      auto loop = [&](ex::Entity entity,
                     components::Player &player,
                     components::Node &node)
        {
          auto node_pos = node.get_pos();
          common::wpos w(node_pos.get_x(), node_pos.get_y(), node_pos.get_z());
          common::cid current_id = w.cid();

          for (auto i = -load; i <= load; ++i)
            for (auto j = -load; j <= load; ++j)
              for (auto k = -load_height; k <= load_height; ++k)
              {
                common::cid iter = common::cid(i, j, k) + current_id;
                if (!(_map.exists(iter) || _map.loading(iter)))
                {
                  events.emit<events::chunk_requested>(iter);
                  _map.loading_started(iter);
                }
              }
        };

      entities.each<components::Player, components::Node>(loop);

      fetch_meshed_chunks(entities, events);
    }

    void ChunkLoader::fetch_meshed_chunks(ex::EntityManager &entities, ex::EventManager &em)
    {
      // return;
      if (_pipe_from_mesher.size())
      {
        auto result = _pipe_from_mesher.dequeue();
        auto nodepath = _scene.attach_new_node(std::get<1>(result));
        common::cid cid(std::get<0>(result));
        common::cpos cp;
        common::wpos wp(cid, cp);

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

          tex->set_magfilter(Texture::FilterType::FT_nearest_mipmap_nearest);
          tex->set_minfilter(Texture::FilterType::FT_nearest_mipmap_nearest);
          node.set_texture(tex);
        }

        nodepath.set_pos(wp.x(), wp.y(), wp.z());

        auto body = std::get<2>(result);
        nodepath.attach_new_node(body);
        body->set_transform_dirty();

        auto entity = entities.create();
        entity.assign<NodePath>(nodepath);
        entity.assign<common::wpos>(wp);
        entity.assign<common::Chunk::ptr>(std::get<3>(result));

        em.emit<events::chunk_loaded>(entity);
        em.emit<events::bullet_attach>(body);
      }
    }

    void ChunkLoader::receive(const events::server_connected &e)
    {
      _connected = true;
    }

    void ChunkLoader::receive(const events::server_disconnected &e)
    {
      _connected = false;
    }

    void ChunkLoader::receive(const events::chunk_received &e)
    {
      auto _chunk = e.msg;
      _pipe_to_mesher << blocks::common::Chunk::deserialize(_chunk);
    }

    void ChunkLoader::receive(const events::block_update &e)
    {
      auto bpos = e.msg;
      auto _pos = bpos->pos();
      common::wpos pos(_pos->x(), _pos->y(), _pos->z());
      auto entity = _map.get(pos.cid());
      auto chunk = std::shared_ptr<common::Chunk>(entity.component<common::Chunk::ptr>()->get());
      chunk->at(pos.cpos()).replace_by<fbs::Block>(bpos->block());
      _pipe_to_mesher << chunk;
    }
  }
}
