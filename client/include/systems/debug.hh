
#include "entity.hh"
#include "systems/input.hh"

namespace blocks
{
  namespace systems
  {
    struct Debug : public ex::System<Debug>, public ex::Receiver<Debug>
    {
      void configure(ex::EventManager &events);
      void update(ex::EntityManager &entities,
                  ex::EventManager &events,
                  ex::TimeDelta dt);

      void receive(const KeyPress &e);
      void receive(const MouseMove &e);
      void receive(const ex::EntityCreatedEvent &e);
      void receive(const ex::EntityDestroyedEvent &e);
    protected:
    };
  }
}
