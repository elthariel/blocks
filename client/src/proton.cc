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
    // _conn.open_sender(_url.path());
  }

  void Proton::on_sendable(proton::event &e)
  {
    // Fetch stuff from queue
    // if (_sent)
    //   return ;
    // std::cout << "Is sendable !" << std::endl;
    // proton::sender sender = e.sender();
    //
    // proton::message msg;
    // // std::map<std::string, int> m;
    // std::string lol("Test");
    //
    // msg.id(_sent);
    // msg.body(lol);
    // sender.send(msg);
    // _sent++;
  }

  void Proton::on_message(proton::event &e)
  {
    // std::cout << "Message !" << e.content() << std::endl;
    proton::message& msg = e.message();

  //  if (msg.id().get<uint64_t>() < _received) {
  //      return; // Ignore duplicate
  //  }

    std::cout << "Message" << msg.body() << std::endl;
  //  _received++;

  //  e.receiver().close();
  //  e.connection().close();
  //  }
    // Puts the message back in the queue
  }

  void Proton::on_unhandled_error (proton::event &e, const proton::condition &c)
  {
    std::cout << "Error: " << c.name()
              << " : " << c.description()
              << std::endl;
  }

}
