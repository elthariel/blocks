#pragma once

#include "session.hh"
#include "mruby.hh"
#include "mruby_value.hh"

#include <texturePool.h>

namespace blocks
{
  class Textures: public SessionObject<Textures>
  {
  public:
    Textures();

    PT(Texture) for_block_id(unsigned bid) { return PT(Texture)(); }
    bool parsed() { return _parsed; }
  protected:
    bool parse_dsl();

    ruby::mruby _mrb;
    ruby::value _mats;
    bool _parsed = false;
  };
}
