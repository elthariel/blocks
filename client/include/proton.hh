#pragma once

#include "common/util.hh"
#include "common/thread.hh"
#include "pipe.hh"

#include <proton/container.hpp>
#include <proton/event.hpp>
#include <proton/handler.hpp>
#include <proton/url.hpp>

#include <atomic>
#include <thread>

namespace blocks
{
  class Proton : public proton::handler, public Thread
  {
  public:
    typedef Pipe<std::string> pipe;

    Proton(const proton::url& u) : _url(u) {}
    Proton(const std::string &hostname,
           unsigned port,
           const std::string &user,
           const std::string &password);

    void on_start(proton::event &e);
    void on_sendable(proton::event &e);
    void on_message(proton::event &e);
    void on_unhandled_error (proton::event &e, const proton::condition &c);

    virtual void stop();

    pipe        input, output;
  protected:
    virtual void main();

    proton::url _url;
    proton::connection _conn;
    uint64_t _sent = 0;
    uint64_t _received = 0;
  };
}
