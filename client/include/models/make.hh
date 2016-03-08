#pragma once

// blocks::models::make<Cube>(sdf, sdf, sdf);

#include <geomNode.h>

#include <string>
#include <utility>

namespace blocks
{
  namespace models
  {
    template <class Factory, typename ... Args>
    PT(GeomNode) make(std::string name, Args && ... args)
    {
      Factory f(std::forward<Args>(args) ...);
      return f.make(name);
    }
  }
}
