#include "game.hh"
#include "mruby.hh"
// #include "proton.hh"

#include <iostream>
#include <string>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

// #include "Bus.hh"

using namespace std;
using namespace blocks;

static void chdir_to_bin(char *name)
{
  auto bin_folder = boost::filesystem::system_complete(name).parent_path();
  std::cout << "Changing directory to " << bin_folder << std::endl;
  boost::filesystem::current_path(bin_folder);
}

// class Test
// {
// public:
//   void dispatch(uint8_t *message)
//   {
//     std::cout << "Message!" << std::endl;
//   }
// };

int main(int ac, char **av)
{
  // Test test;
  // Bus<Test> bus("localhost", 5672, "trololo", &test);
  //
  // bus.subscribe("test");
  //
  // common::wpos pos(1, 2, 3);
  // bus.emit("test", Protocole::create_message(fbs::Action::Action_ASK_CHUNK,
  //                                          fbs::AType::AType_PosObj, &pos));

  if (ac != 3)
  {
    std::cerr << "Usage : ./client IP PORT" << std::endl;
    return (0);
  }

  chdir_to_bin(av[0]);
  //
  blocks::Game game(ac, av);
  game.start();

  // ruby::mruby mrb("../ruby/");
  // auto v = mrb.run("client/textures.rb");
  // auto h = v.as_hash();

  // h.each([] (ruby::value k, ruby::value v)
  //        {
  //          std::cout << k.as_string() << ":" << v.as_string() << std::endl;
  //        });

  return 0;
}
