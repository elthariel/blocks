#pragma once

// Enable thread safety.
#define ELPP_THREAD_SAFE 1
// Let's use std::thread, as the rest of the code
#define ELPP_FORCE_USE_STD_THREAD 1
// Yay for pretty printing of stl containers !
#define ELPP_STL_LOGGING 1

#include <easylogging++.h>

namespace blocks
{
  namespace logging
  {
    void configure(int &ac, char **&av);
  }
}
