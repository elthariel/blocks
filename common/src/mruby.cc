#include "mruby.hh"
#include "mruby_value.hh"

#include <mruby/string.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

using namespace std;

namespace blocks
{
  namespace ruby
  {
    mruby::mruby(const std::string &base_path)
      :_base_path(base_path)
    {
      _mrb = mrb_open();
      assert(_mrb != nullptr);
    }

    mruby::~mruby()
    {
      mrb_close(_mrb);
    }

    bool mruby::operator==(const mruby &other)
    {
      return _mrb == other._mrb;
    }

    bool mruby::operator!=(const mruby &other)
    {
      return _mrb != other._mrb;
    }

    value mruby::run(const std::string &path)
    {
      auto full_path = _base_path + "/" + path;
      ifstream rb(full_path);
      ostringstream code;

      if (!rb.is_open())
        cerr << string("Unable to open " + full_path) << endl;

      code << rb.rdbuf();
      rb.close();

      return value(*this, mrb_load_string(_mrb, code.str().c_str()));
    }

    value mruby::eval(const std::string &code)
    {
      return value(*this, mrb_load_string(_mrb, code.c_str()));
    }

    value mruby::exception()
    {
      if (_mrb->exc == nullptr) return value(*this);

      value res(*this, mrb_obj_value(_mrb->exc));

      _mrb->exc = nullptr;

      return res;
    }

    value mruby::root()
    {
      return value(*this, mrb_obj_value(_mrb->top_self));
    }

    mrb_state *mruby::unwrap()
    {
      return _mrb;
    }

    value mruby::make()
    {
      return value(*this);
    }

    value mruby::make(bool b)
    {
      return value(*this, mrb_bool_value(b));
    }

    value mruby::make(mrb_int i)
    {
      return value(*this, mrb_fixnum_value(i));
    }

    value mruby::make(float f)
    {
      return value(*this, mrb_float_value(_mrb, f));
    }

    value mruby::make(std::string s)
    {
      return value(*this, mrb_str_new_cstr(_mrb, s.c_str()));
    }

  }
}
