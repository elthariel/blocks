
#include <duktape.h>

#include <string>

namespace blocks
{
  class DukTape
  {
  public:
    DukTape();
    ~DukTape();

    DukTape(const DukTape&) = delete;

    void eval(const std::string&);

  protected:
    duk_context *_ctx;
  };
}
