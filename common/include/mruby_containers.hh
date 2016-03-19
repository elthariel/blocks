#pragma once

#include "mruby_value.hh"

#include <mruby/array.h>
#include <mruby/hash.h>

namespace blocks
{
  namespace ruby
  {
    class array : public value
    {
    public:
      class iterator
      {
      public:
        iterator() = delete;
        iterator(array &a, mrb_int idx = 0);

        iterator& operator++();
        bool operator==(const iterator &iter);
        iterator &operator=(value v);
        value operator*();

        operator mrb_int();

      protected:
        array &_array;
        mrb_int _idx;
      };

      using value::value;

      value at(mrb_int idx);
      void set(mrb_int idx, value v);
      mrb_int length();
      iterator begin();
      iterator end();
    };

    class hash : public value
    {
    public:
      // typedef std::pair<value, value> kv;
      using value::value;

      array keys();
      value get(value key);
      void set(value key, value v);
      void each(std::function<void (value, value)> f);
    protected:
    };

  }
}
