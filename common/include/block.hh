#pragma once

#include <cstdint>
#include "chunk_generated.h"

namespace blocks {
  class Block
  {
    enum Flag : uint32_t
    {
      AIR            = 1 << 0,
      TRANSPARENT    = 1 << 1
    };

  public:
    Block(uint16_t id = 0, uint16_t variant = 0, bool _air = false,
          bool _transparent = false)
      :_id(id), _variant(variant), _flags(0)
    {
      air(_air);
      transparent(_transparent);
    }

    uint16_t id() const { return _id; }
    void id(uint16_t i) { _id = i; }

    uint16_t variant() const { return _variant; }
    void variant(uint16_t v) { _variant = v; }

    bool air() const { return get_flag(AIR); }
    void air(bool set) { set_flag(AIR, set); }

    bool transparent() const { return get_flag(TRANSPARENT); }
    void transparent(bool set) { set_flag(TRANSPARENT, set); }

    blocks::fbs::Block serialize() const {
      blocks::fbs::Block block(id(), variant(), air(), transparent());
      return block;
    }

  protected:
    uint16_t _id;
    uint16_t _variant;
    uint32_t _flags;

    bool get_flag(Flag f) const { return _flags & f; }
    void set_flag(Flag f, bool set) {
      if (set) {
        _flags |= f;
      } else {
        _flags &= ~f;
      }
    }
  };
}
