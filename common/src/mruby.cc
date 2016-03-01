
#include "mruby.hh"

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

    value mruby::run(const std::string &path)
    {
      auto full_path = _base_path + "/" + path;
      ifstream rb(full_path);
      ostringstream code;

      if (!rb.is_open())
        cerr << string("Unable to open " + full_path) << endl;

      code << rb.rdbuf();
      rb.close();

      return value(_mrb, mrb_load_string(_mrb, code.str().c_str()));
    }

    std::string value::as_string()
    {
      if (!is_string())
        return std::string();

      return std::string(mrb_str_to_cstr(_mrb, _value));
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

    array hash::keys()
    {
      return array(_mrb, mrb_hash_keys(_mrb, _value));
    }

    value hash::get(value key)
    {
      return value(_mrb, mrb_hash_get(_mrb, _value, key.unwrap()));
    }

    void hash::each(std::function<void (value, value)> f)
    {
      auto kz = keys();
      for(auto key: kz)
      {
        auto v = get(key);
        f(key, v);
      }
    }

  }
}
