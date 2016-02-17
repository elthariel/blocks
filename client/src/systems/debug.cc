
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
      events.subscribe<ex::EntityCreatedEvent>(*this);
      events.subscribe<ex::EntityDestroyedEvent>(*this);
      events.subscribe<events::key>(*this);
      events.subscribe<events::mouse>(*this);
      events.subscribe<events::server_connected>(*this);
      events.subscribe<events::server_disconnected>(*this);
      events.subscribe<events::load_chunk>(*this);
    }

    void Debug::update(ex::EntityManager &entities,
                       ex::EventManager &events,
                       ex::TimeDelta dt)
    {
    }

    void Debug::receive(const events::key &kp)
    {
      if (getenv("DEBUG_KEYS"))
        cout << "KeyPress: " << kp.type << ":" << kp.code << endl;
    }

    void Debug::receive(const events::mouse &mm)
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

    void Debug::receive(const events::server_connected &e)
    {
      cout << "Server connected" << endl;
    }

    void Debug::receive(const events::server_disconnected &e)
    {
      cout << "Server disconnected" << endl;
    }

    void Debug::receive(const events::load_chunk &e)
    {
      cout << "Requested Chunk: " << string(e.id) << endl;
    }
  }
}
