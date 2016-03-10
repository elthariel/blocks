#include "common/thread.hh"

#include <chrono>
#include <iostream>

using namespace std;

namespace blocks
{
  Thread::~Thread()
  {
    stop();
    if (_thread)
      _thread->join();
  }

  void Thread::start()
  {
    if (!_thread) {
      _running = true;
      _thread  = unique_ptr<thread>(new thread([&] { main(); }));
    }
  }

  void Thread::join()
  {
    if (_running)
      _thread->join();
  }


  void Thread::main()
  {
    while(_running)
      main_loop();
  }

  void Thread::main_loop()
  {
    cout << "Useless thread going to sleep..." << endl;
    this_thread::sleep_for (chrono::seconds(10));
  }
}
