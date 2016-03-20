#pragma once

#include "common/util.hh"

#include <memory>
#include <array>

#ifndef SESSION_STORAGE_SIZE
# define SESSION_STORAGE_SIZE 32
#endif

namespace blocks
{
  // Base type for the Session Objects. It provides a non templated base type
  // which pointer will be used for storage.
  class BaseSessionObject : public nocopy
  {
  public:
    // Type for the session object storage index
    typedef size_t type_index;
    typedef std::shared_ptr<BaseSessionObject> ptr;

    virtual ~BaseSessionObject() {}

  protected:
    static type_index __storage_index;
  };

  // This templated class allow us to generate a unique storage index per child.
  // This is shamelessly stolen from entityX
  template <class Derived>
  class SessionObject : public BaseSessionObject
  {
  public:
    static type_index storage_index()
    {
      static type_index index = __storage_index++;
      return index;
    }
  };

  template <class Derived>
  class Session : public nocopy
  {
    static Derived *_instance;

  public:
    typedef std::array<BaseSessionObject::ptr, SESSION_STORAGE_SIZE>
      storage_type;

    static Derived &instance()
    {
      if (_instance == nullptr)
        std::cerr
          << "Trying to access session instance before it's initialized."
          << "I'm so disappointed I'm going to crash !" << std::endl;

      return *_instance;
    }

    template <class T>
    static std::shared_ptr<T> &service()
    {
      return instance().template __service<T>();
    }

    template <class T, typename... Args>
    static void make_service(Args &&... args)
    {
      instance().template __make_service<T>(std::forward<Args>(args)...);
    }

  protected:
    static void instance(Derived *s)
    {
      if (_instance != nullptr)
        std::cerr
          << "Trying to intialize already initialized session instance. "
          << "Please hold the line while we corrupt program's memory :)"
          << std::endl;

      _instance = s;
    }

    template <class T>
    std::shared_ptr<T> &__service()
    {
      auto idx = T::storage_index();
      if (_storage[idx])
        return std::static_pointer_cast<T>(_storage[idx]);
      else
      {
        std::cerr << "Accessing a non initialized Session Object" << std::endl;
        return std::shared_ptr<T>();
      }
    }

    template <class T, typename... Args>
    void __make_service(Args &&... args)
    {
      auto idx = T::storage_index();

      if (_storage[idx]) {
        auto so       = std::make_shared<T>(std::forward<Args>(args)...);
        _storage[idx] = so;
      }
    }

    storage_type _storage;
  };

  template <class Derived>
  Derived *Session<Derived>::_instance = nullptr;
}
