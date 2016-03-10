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
    : Thread(true),
      _input_pipe(_input),
      _output_pipe(_output)
  {
  }

  MeshingThread::~MeshingThread()
  {
  }

  void MeshingThread::process_chunk(common::Chunk::ptr chunk)
  {
    // boost::timer::auto_cpu_timer t(6, "Chunk meshing: %w seconds\n");
    // cout << "Meshing thread received a chunk: "
    //      <<  string(chunk->id()) << endl;

    GreedyMesher mesher(chunk);
    auto out = mesher.mesh();
    _output_pipe << MeshingThread::result(chunk->id(),
                                          out.first,
                                          out.second,
                                          chunk);
  }

  void MeshingThread::main_loop()
  {
    common::Chunk::ptr chunk = _input_pipe.wait_for_data(500, _running);
    if (chunk)
      process_chunk(chunk);
  }
}
