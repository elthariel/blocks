
#include "systems/debug.hh"
#include "systems/input.hh"

#include <iostream>
#include <cstdlib>

using namespace std;

namespace blocks
{
  namespace systems
  {
    void Debug::configure(ex::EventManager &events)
    {
      events.subscribe<KeyPress>(*this);
      events.subscribe<MouseMove>(*this);
      events.subscribe<ex::EntityCreatedEvent>(*this);
      events.subscribe<ex::EntityDestroyedEvent>(*this);
    }

    void Debug::update(ex::EntityManager &entities,
                       ex::EventManager &events,
                       ex::TimeDelta dt)
    {
    }

    void Debug::receive(const KeyPress &kp)
    {
      if (getenv("DEBUG_KEYS"))
        cout << "KeyPress: " << kp.type << ":" << kp.code << endl;
    }

    void Debug::receive(const MouseMove &mm)
    {
      if (getenv("DEBUG_MOUSE"))
        cout << "MouseMove: " << mm.position.get_x() << ":"
             << mm.position.get_y() << endl;
    }

    void Debug::receive(const ex::EntityCreatedEvent &e)
    {
      if (getenv("DEBUG_ENTITY"))
        cout << "Created entity: " << e.entity << endl;
    }

    void Debug::receive(const ex::EntityDestroyedEvent &e)
    {
      if (getenv("DEBUG_ENTITY"))
        cout << "Destroyed entity: " << e.entity << endl;
    }
  }
}
