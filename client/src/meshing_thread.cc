
#include "meshing_thread.hh"
#include "mesher.hh"

#include <boost/timer/timer.hpp>
#include <iostream>
#include <chrono>
#include <string>

using namespace std;

namespace blocks {
  MeshingThread::MeshingThread()
  : _running(true),
    _thread([=] { thread_loop(); })
  {
  }

  MeshingThread::~MeshingThread()
  {
    stop();
    _thread.join();
  }

  size_t MeshingThread::chunk_queue_put(Chunk::ptr c)
  {
    lock_guard<mutex> lock(_in_mutex);

    _in_queue.push(c);
    _work_signal.notify_one();

    return _in_queue.size();
  }

  size_t MeshingThread::chunk_queue_size()
  {
    lock_guard<mutex> lock(_in_mutex);

    return _in_queue.size();
  }

  size_t MeshingThread::mesh_queue_put(MeshingThread::result n)
{
    lock_guard<mutex> lock(_out_mutex);

    _out_queue.push(n);

    return _out_queue.size();
  }

  MeshingThread::result MeshingThread::mesh_queue_get()
  {
    lock_guard<mutex> lock(_out_mutex);

    auto res = _out_queue.front();
    _out_queue.pop();

    return res;
  }

  size_t MeshingThread::mesh_queue_size()
  {
    lock_guard<mutex> lock(_out_mutex);

    return _out_queue.size();
  }

  void MeshingThread::stop()
  {
    // cout << "Stopping meshing thread." << endl;
    _running = false;
  }

  void MeshingThread::process_chunk(Chunk::ptr chunk)
  {
    // boost::timer::auto_cpu_timer t(6, "Chunk meshing: %w seconds\n");
    // cout << "Meshing thread received a chunk: "
    //      <<  string(chunk->id()) << endl;

    GreedyMesher mesher(chunk);
    MeshingThread::result result(chunk->id(), mesher.mesh());
    mesh_queue_put(result);
  }

  Chunk::ptr MeshingThread::wait_for_data()
  {
    unique_lock<mutex> lock(_in_mutex);
    while (_in_queue.empty() && _running)
      _work_signal.wait_for(lock, chrono::milliseconds(500));

    if (_running)
    {
      Chunk::ptr chunk = _in_queue.front();
      _in_queue.pop();
      return chunk;
    }
    else
      return Chunk::ptr();
  }

  void MeshingThread::thread_loop()
  {
    cout << "Started meshing thread." << endl;

    while(_running) {
      Chunk::ptr chunk = wait_for_data();
      if (chunk)
        process_chunk(chunk);
    }

    cout << "Meshing thread exiting" << endl;
  }

}
