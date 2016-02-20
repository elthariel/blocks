
#include "meshing_thread.hh"
#include "mesher.hh"

#include <boost/timer/timer.hpp>
#include <iostream>
#include <chrono>
#include <string>

using namespace std;

namespace blocks {
  MeshingThread::MeshingThread(Pipe<common::Chunk::ptr> &_input,
                               Pipe<MeshingThread::result> &_output)
  : _running(true),
    _thread([=] { thread_loop(); }),
    _input_pipe(_input), _output_pipe(_output)
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

  void MeshingThread::process_chunk(common::Chunk::ptr chunk)
  {
    // boost::timer::auto_cpu_timer t(6, "Chunk meshing: %w seconds\n");
    // cout << "Meshing thread received a chunk: "
    //      <<  string(chunk->id()) << endl;

    GreedyMesher mesher(chunk);
    MeshingThread::result result(chunk->id(), mesher.mesh());

    _output_pipe << result;
  }

  void MeshingThread::thread_loop()
  {
    cout << "Started meshing thread." << endl;

    while(_running) {
      common::Chunk::ptr chunk = _input_pipe.wait_for_data(500, _running);
      if (chunk)
        process_chunk(chunk);
    }

    cout << "Meshing thread exiting" << endl;
  }

}
