#pragma once

#include <tuple>
#include <cstdint>

namespace blocks {
  typedef std::tuple<int64_t, int64_t, int64_t> base_position;

  struct pos : public base_position {
    using base_position::base_position;

    inline int64_t &x() { return std::get<0>(*this); }
    inline int64_t &y() { return std::get<1>(*this); }
    inline int64_t &z() { return std::get<2>(*this); }
    inline const int64_t &x() const { return std::get<0>(*this); }
    inline const int64_t &y() const { return std::get<1>(*this); }
    inline const int64_t &z() const { return std::get<2>(*this); }
  };

  struct cid: public pos {
    using pos::pos;
  };

  struct cpos: public pos {
    using pos::pos;

    cpos(size_t idx);
    size_t to_idx() const;
    bool valid() const;
  };

  struct wpos: public pos {
    using pos::pos;

  public:
    wpos(const blocks::cid &_cid, const blocks::cpos &_cpos);

    cid cid() const;
    cpos cpos() const;
  };

  template <typename T>
  T operator+ (const T& a, const T& b) {
    return T(
      a.x() + b.x(),
      a.y() + b.y(),
      a.z() + b.z()
    );
  }

  template cpos operator+<cpos>(const cpos &, const cpos &);
}
