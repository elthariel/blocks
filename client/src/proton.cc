#include "proton.hh"

#include <boost/format.hpp>

namespace blocks
{
  Proton::Proton(const std::string &hostname,
                 unsigned port,
                 const std::string &user,
                 const std::string &password)
    : _url((boost::format("%1%:%2%") % hostname % port).str())
  {
  }

  void Proton::stop()
  {
    if(running())
      {
        _conn.close();
        Thread::stop();
      }
  }

  void Proton::main()
  {
    proton::container(*this).run();
    _running = false;
  }

  void Proton::on_start(proton::event &e)
  {
    _conn = e.container().connect(_url);
    _conn.open_receiver(_url.path());
    _conn.open_sender(_url.path());
  }

  void Proton::on_sendable(proton::event &e)
  {
    // Fetch stuff from queue
  }

  void Proton::on_message(proton::event &e)
  {
    // Puts the message back in the queue
  }

  void Proton::on_unhandled_error (proton::event &e, const proton::condition &c)
  {
    std::cout << "Error: " << c.name()
              << " : " << c.description()
              << std::endl;
  }

}
