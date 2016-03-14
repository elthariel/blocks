#pragma once

#include <cstdint>
#include "chunk_generated.h"
#include "position.hh"

namespace blocks {
  class Block
  {
    enum Flag : uint32_t
    {
      AIR            = 1 << 0,
      TRANSPARENT    = 1 << 1,
      LIGHT          = 1 << 2
    };

  public:
    Block(uint16_t id = 0, uint16_t variant = 0, bool _air = false,
          bool _transparent = false, int _light = 0)
      :_id(id), _variant(variant), _flags(0)
    {
      air(_air);
      light(_light);
      transparent(_transparent);
    }

    uint16_t id() const { return _id; }
    void id(uint16_t i) { _id = i; }

    uint16_t variant() const { return _variant; }
    void variant(uint16_t v) { _variant = v; }

    bool air() const { return get_flag(AIR); }
    void air(bool set) { set_flag(AIR, set); }

    bool light() const { return get_flag(LIGHT); }
    void light(bool set) { set_flag(LIGHT, set); }

    bool transparent() const { return get_flag(TRANSPARENT); }
    void transparent(bool set) { set_flag(TRANSPARENT, set); }

    template <class T>
    void replace_by(T const *block)
    {
      id(block->id());
      air(block->air());
      light(block->light());
      transparent(block->transparent());
      variant(block->variant());
    }

    flatbuffers::Offset<fbs::Block>  serialize(flatbuffers::FlatBufferBuilder &builder) const {
      return blocks::fbs::CreateBlock(builder, id(), variant(), air(), transparent(), light());
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

  class BlockPos
  {
    public:
      BlockPos(Block &block, common::wpos &wpos) : _block(block), _wpos(wpos) {}

      flatbuffers::Offset<fbs::BlockPos> serialize(flatbuffers::FlatBufferBuilder &builder)
      {
        auto pos = fbs::Pos(_wpos.x(), _wpos.y(), _wpos.z());
        auto block = _block.serialize(builder);
        return fbs::CreateBlockPos(builder, block, &pos);
      }

    private:
      Block &_block;
      common::wpos &_wpos;
  };
}
