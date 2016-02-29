#pragma once

#include "session.hh"
#include "mruby.hh"

#include <texturePool.h>

namespace blocks
{
  class Textures: public SessionObject
  {
  public:
    Textures();

    PT(Texture) for_block_id(unsigned bid) {}
  protected:
    ruby::mruby _mrb;
  };
}
