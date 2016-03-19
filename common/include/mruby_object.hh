#pragma once

#include <mruby.h>

#include <string>
#include <array>

namespace blocks
{
  class value;

  namespace ruby
  {
    class object : public value
    {
    public:
      using value::value;

    protected:
    };
  }
}
