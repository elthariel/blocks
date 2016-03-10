#pragma once

#include "common/util.hh"
#include <functional>

#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/array.h>
#include <mruby/hash.h>

#include <string>
#include <functional>

#undef mrb_int

namespace blocks
{
  namespace ruby
  {
    class value;
    class array;
    class hash;

    class mruby : public nocopy
    {
    public:
      mruby(const std::string &base_path);

      value run(const std::string &path);

    protected:
      mrb_state *_mrb;

      std::string _base_path;
    };

    class base_value
    {
    public:
      base_value() = delete;
      base_value(mrb_state *mrb) :_mrb(mrb), _value(mrb_nil_value()) {}
      base_value(mrb_state *mrb, mrb_value v) :_mrb(mrb), _value(v) {}

      operator bool() { return mrb_test(_value); }
      mrb_value unwrap() { return _value; }
    protected:
      mrb_state *_mrb;
      mrb_value _value;
    };

    class value : public base_value
    {
    public:
      using base_value::base_value;

      bool is_nil() { return mrb_nil_p(_value); }
      bool is_undef() { return mrb_undef_p(_value); }
      bool is_fixnum() { return mrb_fixnum_p(_value); }
      bool is_string() { return mrb_string_p(_value); }
      bool is_symbol() { return mrb_symbol_p(_value); }
      bool is_float() { return mrb_float_p(_value); }
      bool is_array() { return mrb_array_p(_value); }
      bool is_hash() { return mrb_hash_p(_value); }

      mrb_int as_int() { return mrb_fixnum(_value); }
      mrb_float as_float() { return mrb_float(_value); }
      mrb_sym as_sym() { return mrb_symbol(_value); }

      std::string as_string();
      array as_array();
      hash as_hash();
    };

    class array : public base_value
    {
    public:
      class iterator
      {
      public:
        iterator() = delete;
        iterator(array &a, mrb_int idx = 0) : _array(a), _idx(idx) {}
        iterator& operator++() { ++_idx; return *this; }
        bool operator==(const iterator &iter) { return _idx == iter._idx; }
        value operator*() { return _array.at(_idx); }
        operator mrb_int() { return _idx; }

      protected:
        array &_array;
        mrb_int _idx;
      };

      using base_value::base_value;

      mrb_int length() { return mrb_ary_len(_mrb, _value); }
      value at(mrb_int idx) { return value(_mrb, mrb_ary_ref(_mrb, _value, idx)); }
      iterator begin() { return iterator(*this, 0); }
      iterator end() { return iterator(*this, length()); }
    };

    class hash : public base_value
    {
    public:
      // typedef std::pair<value, value> kv;
      using base_value::base_value;

      array keys();
      value get(value key);
      void each(std::function<void (value, value)> f);
    protected:
    };
  }
}
