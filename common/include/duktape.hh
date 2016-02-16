
#include <duktape.h>

#include <string>
#include <cstdint>
#include <memory>

namespace blocks
{
  class DukTapeStack
  {
  public:
    typedef std::shared_ptr<DukTapeStack> ptr;

    DukTapeStack() = delete;
    DukTapeStack(duk_context *ctx);

    std::string get_safe_string(duk_idx_t index);
    int64_t get_int(duk_idx_t index);
    bool get_bool(duk_idx_t index);

    int64_t result_int();
    bool result_bool();

  protected:
    duk_context *_ctx;
  };

  class DukTape
  {
  public:
    DukTape();
    ~DukTape();

    DukTape(const DukTape&) = delete;

    bool eval_str(const std::string&);
    bool eval(const std::string&);

    DukTapeStack::ptr stack = nullptr;
  protected:
    duk_context *_ctx = nullptr;
  };
}
