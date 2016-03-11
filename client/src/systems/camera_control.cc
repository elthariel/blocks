#include "systems/camera_control.hh"
#include "events/player.hh"

#include <bulletCharacterControllerNode.h>

namespace blocks
{
  namespace systems
  {
    void CameraControl::configure(ex::EventManager &events)
    {
      events.subscribe<events::key>(*this);
      events.subscribe<events::mouse>(*this);
    }

    float cam_sensitivity = 0.01;
    float _move_speed = 0.2;
    void CameraControl::update(ex::EntityManager &entities,
                               ex::EventManager &events,
                               ex::TimeDelta dt)
    {
      auto lambda = [&](ex::Entity entity,
                        components::Player &player,
                        components::Node &node)
      {
        auto c_node = dynamic_cast<BulletCharacterControllerNode *>(node.node());
        auto cam = node.get_child(0);
        LVecBase3 hpr = cam.get_hpr();

        if (_do_fly)
          {
            _do_fly = false;
            if (c_node->get_gravity() > 0.1)
              c_node->set_gravity(0);
            else
              c_node->set_gravity(9.81);
          }

        if (_do_jump)
          {
            _do_jump = false;
            if (c_node->can_jump())
              c_node->do_jump();
          }

        hpr.add_x(_mouse_offset.get_x() * cam_sensitivity * -1.0);
        hpr.add_y(_mouse_offset.get_y() * cam_sensitivity);

        cam.set_hpr(hpr);

        LPoint3 _pos = node.get_pos();
        LPoint3 offset(0);
        auto forward = cam.get_quat().get_forward();
        auto right = cam.get_quat().get_right();

        offset += right * _move_speed * _move.get_x();
        offset += forward * _move_speed * _move.get_y();
        offset += _move_absolute * _move_speed;

        auto final_pos = _pos + offset;
        node.set_fluid_pos(final_pos);
      };

      entities.each<components::Player,
                    components::Node>(lambda);

      _mouse_offset.set(0, 0);
      _move.set(0, 0, 0);
      _move_absolute.set(0, 0, 0);
    }

    void CameraControl::receive(const events::key &e)
    {
      if (e.type == events::key::ktype::DOWN || e.type == events::key::ktype::REPEAT)
      {
        switch (e.code)
        {
        case events::key::kcode::MOVE_RIGHT:
          _move.add_x(1);
          break;
        case events::key::kcode::MOVE_LEFT:
          _move.add_x(-1);
          break;
        case events::key::kcode::MOVE_FORWARD:
          _move.add_y(1);
          break;
        case events::key::kcode::MOVE_BACKWARD:
          _move.add_y(-1);
          break;
        default:
          break;
        }
      }
      if (e.type == events::key::ktype::DOWN)
        switch(e.code)
          {
          case events::key::kcode::MOVE_JUMP:
            _do_jump = true;
            break;
          case events::key::kcode::MOVE_FLY:
            _do_fly = true;
          default:
            break;
          }
    }

    void CameraControl::receive(const events::mouse &e)
    {
      _mouse_offset = e.offset;
    }
  }
}
