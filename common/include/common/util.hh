#pragma once

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
