#include "common/logging.hh"

namespace blocks
{
  namespace logging
  {
    void configure(int &ac, char **&av)
    {
      START_EASYLOGGINGPP(ac, av);
    }
  }
}
