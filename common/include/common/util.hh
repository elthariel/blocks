#pragma once

#include <iostream>

#define DEBUG(msg) std::cout << msg << std::endl

namespace blocks
{
  struct nocopy
  {
    nocopy() = default;
    ~nocopy() = default;

    nocopy(const nocopy &) = delete;
    nocopy& operator = (const nocopy &) = delete;
  };
}
