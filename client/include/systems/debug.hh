#pragma once

#include "entity.hh"
#include "events/input.hh"
#include "events/map.hh"
#include "events/network.hh"

namespace blocks
{
  class Game;
  namespace systems
  {
    struct Debug : public ex::System<Debug>, public ex::Receiver<Debug>
    {
      Debug(Game &);
      void configure(ex::EventManager &events);
      void update(ex::EntityManager &entities,
                  ex::EventManager &events,
                  ex::TimeDelta dt);

      void receive(const ex::EntityCreatedEvent &e);
      void receive(const ex::EntityDestroyedEvent &e);
      void receive(const events::key &e);
      void receive(const events::mouse &e);
      void receive(const events::server_connected &e);
      void receive(const events::server_disconnected &e);
      void receive(const events::load_chunk &e);
      protected:
      Game &_game;
    };
  }
}
