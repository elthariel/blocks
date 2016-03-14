#include "systems/physics.hh"
#include "components/basic.hh"

#include <nodePathCollection.h>
#include <bulletTriangleMesh.h>
#include <bulletTriangleMeshShape.h>
#include <bulletCapsuleShape.h>
#include <bulletCharacterControllerNode.h>
#include <bulletDebugNode.h>
#include <bitMask.h>

#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

namespace blocks
{
  namespace systems
  {
    Physics::Physics(Scene::ptr scene)
      : _world(new BulletWorld()), _scene(scene)
    {
      _world->set_gravity(0, 0, -9.81f);
      _world->set_group_collision_flag(0, 0, false);
      _world->set_group_collision_flag(0, 1, false);
      _world->set_group_collision_flag(0, 2, false);
      _world->set_group_collision_flag(0, 3, false);

      _world->set_group_collision_flag(1, 1, false);
      _world->set_group_collision_flag(1, 2, true);
      _world->set_group_collision_flag(1, 3, true);

      _world->set_group_collision_flag(2, 2, true);
      _world->set_group_collision_flag(2, 3, false);

      _world->set_group_collision_flag(3, 3, false);
    }

    static float height = 2.0;
    static float radius = 0.5;
    void Physics::configure(ex::EntityManager &entities, ex::EventManager &events)
    {
      events.subscribe<events::key>(*this);
      events.subscribe<events::bullet_attach>(*this);
      events.subscribe<events::bullet_remove>(*this);

      auto l = [&] (ex::Entity e,
                    components::Player &player,
                    components::Node &node)
        {
          // Create our character node.
          PT(BulletCapsuleShape) c_shape = new BulletCapsuleShape(radius,
                                                                  height -2 * radius);
          PT(BulletCharacterControllerNode) controller =
            new BulletCharacterControllerNode(c_shape, 0.4f, "player");
          _world->attach_character(controller);

          controller->set_gravity(0);
          // Reparent the camera_group under that node
          auto parent = node.get_parent();
          auto char_node = parent.attach_new_node(controller);
          char_node.set_pos(node.get_pos());
          node.set_pos(radius / 2,  radius / 2, height / 2);
          node.reparent_to(char_node);

          // Set collisions mask and shit
          char_node.set_collide_mask(BitMask32::bit(2));

          // Replace the node in the player
          e.remove<components::Node>();
          e.assign<components::Node>(char_node);
        };

      entities.each<components::Player, components::Node>(l);
    }

    void Physics::update(ex::EntityManager &entities,
                         ex::EventManager &events,
                         ex::TimeDelta dt)
    {
      _world->do_physics(10, 10, 1.0 / 180.0);

      auto l = [&](ex::Entity ex,
                   components::Player &player,
                   components::Node &node)
       {
          auto cam = node.get_child(0);
          auto forward = cam.get_quat().get_forward();
          auto pos_from = node.get_pos() + cam.get_pos();
          auto pos_to = pos_from + forward * 10;
          auto hit = _world->ray_test_closest(pos_from, pos_to,
                                              BitMask32::bit(3));

          if (hit.has_hit())
          {
            auto hp = hit.get_hit_pos();
            auto hn = hit.get_hit_normal();
            common::wpos pos(hp.get_x(), hp.get_y(), hp.get_z());

            if (hn.get_x() > 0) pos.x()--;
            if (hn.get_y() > 0) pos.y()--;
            if (hn.get_z() > 0) pos.z()--;

            _scene->aim_cube().show();
            _scene->aim_cube().set_pos(pos.x() - 0.002,
                                       pos.y() - 0.002,
                                       pos.z() - 0.002);

            // std::cout << "Picking something."
            //           << "pos: " << hit.get_hit_pos()
            //           << " normal: " << hit.get_hit_normal()
            //           << std::endl;
          }
          else
            _scene->aim_cube().hide();
        };

      entities.each<components::Player, components::Node>(l);
    }

    void Physics::receive(const events::key &e)
    {

      if (e.type == events::key::ktype::DOWN
          && e.code == events::key::kcode::DEBUG_PHYSICS)
      {
        if (!_debug_loaded)
        {
          PT(BulletDebugNode) bullet_dbg_node = new BulletDebugNode("Debug");
          bullet_dbg_node->show_bounding_boxes(true);
          bullet_dbg_node->show_constraints(true);
          bullet_dbg_node->show_normals(true);
          bullet_dbg_node->show_wireframe(true);

          _debug = _scene->root().attach_new_node(bullet_dbg_node);
          _world->set_debug_node(bullet_dbg_node);

          _debug_loaded = true;
        }

        if (_debug.is_hidden())
        {
          DEBUG("Show physics debug layer");
          _debug.show();
        }
        else
        {
          DEBUG("Hide physics debug layer");
          _debug.hide();
        }
      }
    }

    void Physics::receive(const events::bullet_attach &e)
    {
      _world->attach(e.body);
    }

    void Physics::receive(const events::bullet_remove &e)
    {
      _world->remove(e.body);
    }

    // void Physics::receive(const events::chunk_loaded &e)
    // {
    //   // std::cout << "Loaded chunk: " << e.entity << std::endl;
    //   auto entity(e.entity);
    //   auto node = entity.component<NodePath>();
    //   auto wp = entity.component<common::wpos>();

    //   if (node && wp)
    //   {
    //     PT(BulletRigidBodyNode) rigid_node = new BulletRigidBodyNode("chunk_shape");
    //     PT(BulletTriangleMesh) triangle_mesh = new BulletTriangleMesh();

    //     auto children = node->get_children();
    //     for(auto i = 0; i < children.size(); ++i)
    //     {
    //       std::cout << children.get_path(i) << std::endl;
    //       auto geom_node = DCAST(GeomNode, children.get_path(i).node());
    //       auto geom = geom_node->get_geom(0);
    //       triangle_mesh->add_geom(geom);
    //     }

    //     PT(BulletTriangleMeshShape) chunk_shape =
    //       new BulletTriangleMeshShape(triangle_mesh, false);
    //     rigid_node->add_shape(chunk_shape);

    //     auto np = node->attach_new_node(rigid_node);
    //     rigid_node->set_transform_dirty();
    //     np.set_collide_mask(BitMask32::bit(1));
    //     _world->attach(rigid_node);
    //   }
    //   else
    //     DEBUG("Invalid chunk_loaded event");
    // }
  }
}
