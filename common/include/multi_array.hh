
#include "meta.hh"

#include <boost/multi_array.hpp>
#include <cmath>

namespace blocks {
  template<typename T>
  using array2 = boost::multi_array<T, 2>;

  template<typename T>
  using array3 = boost::multi_array<T, 3>;
}
