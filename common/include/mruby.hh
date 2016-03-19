#pragma once

#include "common/util.hh"

#include <mruby.h>
#include <mruby/compile.h>

#include <string>
#include <functional>

#undef mrb_int

namespace blocks
{
  namespace ruby
  {
    class value;

    class mruby : public nocopy
    {
    public:
      mruby(const std::string &base_path);
      ~mruby();

      bool operator==(const mruby &other);
      bool operator!=(const mruby &other);

      value run(const std::string &path);
      value eval(const std::string &code);

      value exception();
      value root();

      mrb_state *unwrap();

      value make();             // -> nil
      value make(bool b);
      value make(mrb_int i);
      value make(float f);
      value make(std::string s);


    protected:
      mrb_state *_mrb;

      std::string _base_path;
    };

  }
}
