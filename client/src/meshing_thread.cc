
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

    output_pipe << result;
  }

  void MeshingThread::thread_loop()
  {
    cout << "Started meshing thread." << endl;

    while(_running) {
      Chunk::ptr chunk = input_pipe.wait_for_data(500, _running);
      if (chunk)
        process_chunk(chunk);
    }

    cout << "Meshing thread exiting" << endl;
  }

}
