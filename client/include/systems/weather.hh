#pragma once

#include "entity.hh"

namespace blocks
{
  namespace systems
  {
    class Weather
    {
    public:
      void configure(ex::EventManager &events);
      void update(ex::EntityManager &entities,
                  ex::EventManager &events,
                  ex::TimeDelta dt);
    protected:
      long _time_of_day;
    }
  }
}
