#pragma once

template <size_t N, unsigned POW>
struct Power {
  enum { value = N * Power<N, POW - 1>::value };
};

template<size_t N>
struct Power<N, 0> {
  enum { value = 1 };
};
