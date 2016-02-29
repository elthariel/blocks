
#include <systems/physics.hh>
#include <bulletTriangleMesh.h>
#include <bulletTriangleMeshShape.h>
#include <bitMask.h>

#include <iostream>

namespace blocks
{
  namespace systems
  {
    Physics::Physics()
      : _world(new BulletWorld())
    {
      _world->set_gravity(0, 0, -9.81f);
      _world->set_group_collision_flag(0, 0, false);
      _world->set_group_collision_flag(0, 1, false);
      _world->set_group_collision_flag(1, 1, false);
    }

    void Physics::configure(ex::EventManager &events)
    {
      events.subscribe<events::chunk_loaded>(*this);
    }

    void Physics::update(ex::EntityManager &entities,
                         ex::EventManager &events,
                         ex::TimeDelta dt)
    {
      _world->do_physics(10, 10, 1.0 / 180.0);
    }

    void Physics::receive(const events::chunk_loaded &e)
    {
      // std::cout << "Loaded chunk: " << e.entity << std::endl;
      auto entity(e.entity);
      auto node = entity.component<NodePath>();
      auto wp = entity.component<common::wpos>();

      if (node && wp)
      {
        BulletRigidBodyNode *rigid_node = new BulletRigidBodyNode("chunk_shape");
        auto geom_node = DCAST(GeomNode, node->node());
        for(auto i = 0; i < geom_node->get_num_geoms(); i++)
        {
          auto geom = geom_node->get_geom(i);
          BulletTriangleMesh *triangle_mesh = new BulletTriangleMesh();
          triangle_mesh->add_geom(geom);
          BulletTriangleMeshShape *chunk_shape = new BulletTriangleMeshShape(triangle_mesh, false);
          rigid_node->add_shape(chunk_shape);
        }

        auto np = node->attach_new_node(rigid_node);
        np.set_collide_mask(BitMask32(1));
        _world->attach_rigid_body(rigid_node);
      }
      else
        DEBUG("Invalid chunk_loaded event");
    }
  }
}
