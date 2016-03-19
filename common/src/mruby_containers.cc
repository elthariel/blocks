#include "mruby_containers.hh"
#include "mruby_value.hh"

namespace blocks
{
  namespace ruby
  {
    //
    // array::iterator
    //

    array::iterator::iterator(array &a, mrb_int idx)
      : _array(a)
      , _idx(idx)
    {
    }

    array::iterator &array::iterator::operator++()
    {
      ++_idx;
      return *this;
    }

    bool array::iterator::operator==(const iterator &iter)
    {
      return _idx == iter._idx;
    }

    array::iterator &array::iterator::operator=(value v)
    {
      _array.set(_idx, v);
      return *this;
    }

    value array::iterator::operator*() { return _array.at(_idx); }
    array::iterator::operator mrb_int() { return _idx; }


    //
    //  array
    //

    value array::at(mrb_int idx)
    {
      return value(_mrb, mrb_ary_ref(_mrb.unwrap(), _value, idx));
    }

    void array::set(mrb_int idx, value v)
    {
      mrb_ary_set(_mrb.unwrap(), _value, idx, v.unwrap());
    }

    mrb_int array::length() { return mrb_ary_len(_mrb.unwrap(), _value); }
    array::iterator array::begin() { return iterator(*this, 0); }
    array::iterator array::end() { return iterator(*this, length()); }

    //
    // hash
    //

    value hash::get(value key)
    {
      return value(_mrb, mrb_hash_get(_mrb.unwrap(), _value, key.unwrap()));
    }

    void hash::set(value key, value v)
    {
      mrb_hash_set(_mrb.unwrap(), _value, key.unwrap(), v.unwrap());
    }

    void hash::each(std::function<void(value, value)> f)
    {
      auto kz = keys();
      for (auto key : kz) {
        auto v = get(key);
        f(key, v);
      }
    }

    array hash::keys()
    {
      return array(_mrb, mrb_hash_keys(_mrb.unwrap(), _value));
    }
  }
}
