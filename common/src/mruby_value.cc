#include "mruby_value.hh"
#include "mruby_containers.hh"

#include <mruby/string.h>

namespace blocks
{
  namespace ruby
  {
    value::value(const value &other)
      : _mrb(other._mrb)
      , _value(other._value)
    {
    }

    value::value(mruby &mrb, mrb_value v)
      : _mrb(mrb)
      , _value(v)
    {
    }

    value::value(mruby &mrb)
      : value(mrb, mrb_nil_value())
    {
    }

    value::value(mruby &mrb, mrb_sym sym)
      : value(mrb, mrb_symbol_value(sym))
    {
    }

    value &value::operator=(const value &other)
    {
      if (_mrb != other._mrb)
        throw "Different mruby objects";

      _value = other._value;
      return *this;
    }


    value::operator bool() { return as_bool(); }
    mrb_value value::unwrap() { return _value; }

    bool value::is_nil() { return mrb_nil_p(_value); }
    bool value::is_undef() { return mrb_undef_p(_value); }
    bool value::is_fixnum() { return mrb_fixnum_p(_value); }
    bool value::is_string() { return mrb_string_p(_value); }
    bool value::is_symbol() { return mrb_symbol_p(_value); }
    bool value::is_float() { return mrb_float_p(_value); }
    bool value::is_array() { return mrb_array_p(_value); }
    bool value::is_hash() { return mrb_hash_p(_value); }
    bool value::is_exception() { return mrb_exception_p(_value); }

    bool value::as_bool() { return mrb_test(_value); }

    std::string value::as_string()
    {
      if (!is_string())
        return std::string();

      return std::string(mrb_str_to_cstr(_mrb.unwrap(), _value));
    }

    array value::as_array()
    {
      if (!is_array())
        {
          std::cerr << "Not an array. The universe will collapse soon..." << std::endl;
          return array(_mrb);
        }
      return array(_mrb, _value);
    }

    hash value::as_hash()
    {
      if (!is_hash())
        {
          std::cerr << "Not a hash. The fabric of the universe is impacted..." << std::endl;
          return hash(_mrb);
        }
      return hash(_mrb, _value);
    }

    mrb_int value::as_int() { return mrb_fixnum(_value); }
    mrb_float value::as_float() { return mrb_float(_value); }
    mrb_sym value::as_symbol() { return mrb_symbol(_value); }

    std::string value::classname()
    {
      return std::string(mrb_obj_classname(_mrb.unwrap(), _value));
    }

    bool value::respond_to(const std::string &name)
    {
      auto sym = mrb_intern_cstr(_mrb.unwrap(), name.c_str());
      return mrb_respond_to(_mrb.unwrap(), _value, sym);
    }

    bool value::respond_to(value &sym)
    {
      if (!sym.is_symbol())
        return false;

      auto _sym = sym.as_symbol();
      return mrb_respond_to(_mrb.unwrap(), _value, _sym);
    }

  }
}
