
#include "systems/camera_control.hh"
#include "events/player.hh"

namespace blocks
{
  namespace systems
  {
    void CameraControl::configure(ex::EventManager &events)
    {
      events.subscribe<events::key>(*this);
      events.subscribe<events::mouse>(*this);
    }

    float cam_sensitivity = 50.0;
    float move_speed = 0.4;
    void CameraControl::update(ex::EntityManager &entities,
                               ex::EventManager &events,
                               ex::TimeDelta dt)
    {
      auto lambda = [&](ex::Entity entity,
                        components::Player &player,
                        components::Position &position,
                        components::Direction &direction,
                        components::Camera &cam)
      {
        LVecBase3 hpr = cam.get_hpr();

        hpr.add_x(mouse_offset.get_x() * cam_sensitivity * -1.0);
        hpr.add_y(mouse_offset.get_y() * cam_sensitivity);

        cam.set_hpr(hpr);

        LPoint3 _pos = cam.get_pos();
        LPoint3 offset(0);
        auto forward = cam.get_quat().get_forward();
        auto right = cam.get_quat().get_right();

        offset += right * move_speed * move.get_x();
        offset += forward * move_speed * move.get_y();
        offset += move_absolute * move_speed;

        auto final_pos = _pos + offset;
        cam.set_fluid_pos(final_pos);

        position = cam.get_pos();
        direction = cam.get_quat().get_forward();
      };

      entities.each<components::Player,
                    components::Position,
                    components::Direction,
                    components::Camera>(lambda);

      mouse_offset.set(0, 0);
      move.set(0, 0, 0);
      move_absolute.set(0, 0, 0);
    }

    void CameraControl::receive(const events::key &e)
    {
      if (e.type == events::key::ktype::DOWN || e.type == events::key::ktype::REPEAT)
      {
        switch (e.code)
        {
        case events::key::kcode::MOVE_RIGHT:
          move.add_x(1);
          break;
        case events::key::kcode::MOVE_LEFT:
          move.add_x(-1);
          break;
        case events::key::kcode::MOVE_FORWARD:
          move.add_y(1);
          break;
        case events::key::kcode::MOVE_BACKWARD:
          move.add_y(-1);
          break;
        case events::key::kcode::MOVE_JUMP:
          move_absolute.add_z(1);
          break;
        default:
          break;
        }
      }
    }

    void CameraControl::receive(const events::mouse &e)
    {
      mouse_offset = e.offset;
    }
  }
}
