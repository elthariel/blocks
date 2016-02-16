
#include "duktape.hh"

#include <iostream>

extern "C" {
  static void on_error(duk_context *ctx, duk_errcode_t code, const char *msg)
  {
    std::cerr << "DukTape fatal error, code=" << std::to_string(code)
              << ", msg='" << msg << "'" << std::endl;
  }
}

namespace blocks
{
  DukTapeStack::DukTapeStack(duk_context *ctx)
    :_ctx(ctx)
  {
  }

  std::string DukTapeStack::get_safe_string(duk_idx_t index)
  {
    const char *err = duk_safe_to_string(_ctx, index);
    return std::string(err);
  }

  int64_t DukTapeStack::get_int(duk_idx_t index)
  {
    return duk_to_int(_ctx, index);
  }

  bool DukTapeStack::get_bool(duk_idx_t index)
  {
    return duk_to_boolean(_ctx, index);
  }

  int64_t DukTapeStack::result_int()
  {
    auto res = get_int(-1);
    duk_pop(_ctx);
    return res;
  }

  bool DukTapeStack::result_bool()
  {
    auto res = get_bool(-1);
    duk_pop(_ctx);
    return res;
  }

  DukTape::DukTape()
  {
    _ctx = duk_create_heap(NULL, NULL, NULL, NULL, on_error);
    if (_ctx == nullptr)
      throw std::string("Unable to initialize DukTape");

    stack = std::make_shared<DukTapeStack>(_ctx);
  }

  DukTape::~DukTape()
  {
    duk_destroy_heap(_ctx);
  }

  bool DukTape::eval_str(const std::string &str)
  {
    if (duk_peval_string(_ctx, str.c_str()))
    {
      auto err = stack->get_safe_string(-1);
      duk_pop(_ctx);
      std::cerr << "DukTape: js exception: " << err << std::endl;
      return false;
    }
    else
      return true;
  }

  bool DukTape::eval(const std::string &path)
  {
    duk_eval_file(_ctx, path.c_str());
  }

}
