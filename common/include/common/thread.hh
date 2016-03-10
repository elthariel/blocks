#pragma once

#include "common/util.hh"

#include <memory>
#include <thread>
#include <atomic>

namespace blocks
{
  class Thread : public nocopy
  {
  public:
    Thread() : Thread(false) {}
    Thread(bool start_now):_running(false) { if (start_now) start(); }
    ~Thread();

    void start();
    virtual void stop() { _running = false; };
    virtual bool running() { return _running; }
    virtual void join();

  protected:
    virtual void main();
    virtual void main_loop();

    std::unique_ptr<std::thread> _thread;
    std::atomic_bool _running;
  };
}
