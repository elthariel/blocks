
#include "livescript.hh"

namespace blocks
{
  DukTape::DukTape()
  {
    _ctx = duk_create_heap_default();
    if (_ctx == nullptr)
      throw std::string("Unable to initialize DukTape");
  }

  DukTape::~DukTape()
  {
    duk_destroy_heap(_ctx);
  }

  void DukTape::eval(const std::string &str)
  {
    duk_eval_string(_ctx, str.c_str());
  }
}
