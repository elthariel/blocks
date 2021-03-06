#pragma once

#include <tuple>
#include <cstdint>
#include <string>
#include <boost/format.hpp>

#include "chunk_generated.h"

namespace blocks {
  namespace common {
    //template <>
    //using base_position = std::tuple<int64_t, int64_t, int64_t>;
    typedef std::tuple<int64_t, int64_t, int64_t> base_position;

    struct pos : public base_position {
      using base_position::base_position;
      //base_position() = delete;
      constexpr pos() {
        std::get<0>(*this) = 0;
        std::get<1>(*this) = 0;
        std::get<2>(*this) = 0;
      }

      inline int64_t &x() { return std::get<0>(*this); }
      inline int64_t &y() { return std::get<1>(*this); }
      inline int64_t &z() { return std::get<2>(*this); }
      inline const int64_t &x() const { return std::get<0>(*this); }
      inline const int64_t &y() const { return std::get<1>(*this); }
      inline const int64_t &z() const { return std::get<2>(*this); }

      int64_t &operator[](size_t idx) {
        if (idx == 0)
          return std::get<0>(*this);
        else if (idx == 1)
          return std::get<1>(*this);
        else
          return std::get<2>(*this);
      }

      const int64_t &operator[](size_t idx) const
      {
        if (idx == 0)
          return std::get<0>(*this);
        else if (idx == 1)
          return std::get<1>(*this);
        else
          return std::get<2>(*this);
      }

      operator std::string() const
      {
        return boost::str(boost::format("%1%:%2%:%3%") % x() % y() % z());
      }

      template <class T>
      T operator+(const T& other)
      {
        return T(x() + other.x(),
                 y() + other.y(),
                 z() + other.z());
      }

      flatbuffers::Offset<fbs::PosObj> serialize(flatbuffers::FlatBufferBuilder &builder) const
      {
        auto pos = fbs::Pos(x(), y(), z());
        return fbs::CreatePosObj(builder, &pos);
      }
    };

    struct cid: public pos {
      using pos::pos;
    };

    struct cpos: public pos {
      using pos::pos;

      constexpr cpos() {
        std::get<0>(*this) = 0;
        std::get<1>(*this) = 0;
        std::get<2>(*this) = 0;
      }
      cpos(size_t idx);
      size_t to_idx() const;
      bool valid() const;
    };

    struct wpos: public pos {
      using pos::pos;

    public:
      wpos(const blocks::common::cid &_cid, const blocks::common::cpos &_cpos);

      cid cid() const;
      cpos cpos() const;
    };
  }
}
