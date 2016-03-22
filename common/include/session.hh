#pragma once

#include "common/util.hh"

#include <iostream>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>

namespace blocks
{
  class Session;

  class SessionObject: public nocopy
  {
    friend class Session;
  public:
    virtual ~SessionObject() {}
  };

  class Session: public nocopy
  {
  public:
    typedef std::unordered_map<std::type_index, SessionObject *> storage_type;

    ~Session();
    static Session &instance();
    static void destroy();

    template <class T> static T &get()
    {
      return Session::instance()._get<T>();
    }

    template <class T> T &_get()
    {
      auto idx = std::type_index(typeid(T));
      if (_storage[idx] != nullptr)
        return *dynamic_cast<T *>(_storage[idx]);
      else
      {
        std::cerr << "Accessing a non initialized Session Object" << std::endl;
      }
    }

    template <class T, typename ... Args>
    static void make(Args && ... args)
    {
      Session::instance()._make<T>(std::forward<Args>(args) ...);
    }

    template <class T, typename ... Args>
    void _make(Args && ... args)
    {
      auto idx = std::type_index(typeid(T));
      if (_storage[idx] != nullptr)
      {
        T *o = new T(std::forward<Args>(args) ...);
        _storage[idx] = o;
      }
    }

  private:
    static Session *_instance;

    storage_type _storage;
  };
}
