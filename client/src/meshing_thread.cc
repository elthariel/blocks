
#include "meshing_thread.hh"

namespace blocks {
  MeshingThread::MeshingThread()
  : _running(true),
    _thread([=] { thread_loop(); })
  {
  }

  size_t MeshingThread::chunk_queue_put(Chunk::ptr c) {
    std::lock_guard<std::mutex> lock(_in_mutex);

    _in_queue.push(c);
    _work_signal.notify_one();

    return _in_queue.size();
  }

  size_t MeshingThread::chunk_queue_size() {
    std::lock_guard<std::mutex> lock(_in_mutex);

    return _in_queue.size();
  }

  size_t MeshingThread::mesh_queue_put(PT(GeomNode) n) {
    std::lock_guard<std::mutex> lock(_out_mutex);

    _out_queue.push(n);

    return _out_queue.size();
  }

  PT(GeomNode) MeshingThread::mesh_queue_get() {
    std::lock_guard<std::mutex> lock(_out_mutex);

    PT(GeomNode) n = _out_queue.front();
    _out_queue.pop();

    return n;
  }

  size_t MeshingThread::mesh_queue_size() {
    std::lock_guard<std::mutex> lock(_out_mutex);

    return _out_queue.size();
  }

  void MeshingThread::stop() {
    _running = false;
  }

  void MeshingThread::thread_loop() {
    while(_running) {
      std::unique_lock<std::mutex> lock(_in_mutex);
      _work_signal.wait(lock);

      Chunk::ptr chunk = _in_queue.front();
      _in_queue.pop();
      std::cout << "Meshing Thread received a chunk" << std::endl;


      // XXX: Do the meshing here

      lock.unlock();
    }
  }

}
