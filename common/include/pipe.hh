#pragma once

#include <queue>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <chrono>

namespace blocks
{
  // This class represent a unidirectional data pipe from one thread to another one
  template <class T>
  class Pipe
  {
  public:
    typedef std::queue<T> queue;
    typedef std::shared_ptr<T> ptr;
    typedef std::unique_lock<std::mutex> unique_lock;
    typedef std::lock_guard<std::mutex> lock_guard;

    Pipe(const T invalid_value = T())
      : _invalid_value(invalid_value)
    {
    }

    Pipe(const Pipe &) = delete;

    size_t size()
    {
      lock_guard guard(_mutex);
      return _queue.size();
    }

    size_t enqueue(T value)
    {
      size_t size;
      {
        lock_guard guard(_mutex);
        _queue.push(value);
        size = _queue.size();
      }
      notify();

      return size;
    }

    Pipe<T> &operator<<(T value)
    {
      enqueue(value);
      return *this;
    }

    T dequeue()
    {
      lock_guard guard(_mutex);
      return dequeue_nolock();
    }

    Pipe<T> &operator>>(T &value)
    {
      value = dequeue();
      return *this;
    }

    T wait_for_data(unsigned long block_time_ms, std::atomic_bool &running)
    {
      unique_lock lock(_mutex);
      while (_queue.empty() && running)
      {
        _cond.wait_for(lock, std::chrono::milliseconds(block_time_ms));
      }

      if (running)
        return dequeue_nolock();
      else
        return _invalid_value;
    }

  protected:
    T dequeue_nolock()
    {
      T result = _queue.front();
      _queue.pop();
      return result;
    }

    void notify()
    {
      _cond.notify_one();
    }

    queue _queue;
    std::mutex _mutex;
    std::condition_variable _cond;
    const T _invalid_value;
  };
}
