#pragma once

#include "mruby.hh"

#include <array>

namespace blocks
{
  namespace ruby
  {
    class array;
    class hash;
    class value;

    template <unsigned i>
    using arguments = std::array<value, i>;

    // Base class for all of mruby's values (scalar/sym/hash/array/object/...)
    class value
    {
    public:
      value() = delete;
      value(const value &other);
      value(mruby &mrb);
      value(mruby &mrb, mrb_value v);
      value(mruby &mrb, mrb_sym sym);
      value &operator=(const value &other);

      operator bool();
      mrb_value unwrap();

      bool is_nil();
      bool is_undef();
      bool is_fixnum();
      bool is_string();
      bool is_symbol();
      bool is_float();
      bool is_array();
      bool is_hash();
      bool is_exception();

      bool as_bool();
      mrb_int as_int();
      mrb_float as_float();
      mrb_sym as_symbol();
      std::string as_string();
      array as_array();
      hash as_hash();

      std::string classname();
      std::string inspect();
      bool respond_to(const std::string &name);
      bool respond_to(value &sym);

      template <unsigned argc>
      value send(const std::string &name, arguments<argc> argv)
      {
        mrb_value argv_unwrapped[argc];
        auto sym = mrb_intern_cstr(_mrb.unwrap(), name.c_str());

        for(unsigned i = 0; i < argv.size(); i++)
          argv_unwrapped[i] = argv[i].unwrap();

        return value(
          _mrb,
          mrb_funcall_argv(_mrb.unwrap(), _value, sym, argc, argv_unwrapped));
      }

    protected:
      mruby &_mrb;
      mrb_value _value;
    };
  }
}
