#include "game.hh"
#include "mruby.hh"

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

using namespace std;
using namespace blocks;

static void chdir_to_bin(char *name)
{
  auto bin_folder = boost::filesystem::system_complete(name).parent_path();
  std::cout << "Changing directory to " << bin_folder << std::endl;
  boost::filesystem::current_path(bin_folder);
}

int main(int ac, char **av)
{
  if (ac != 3)
  {
    std::cerr << "Usage : ./client IP PORT" << std::endl;
    return (0);
  }

  chdir_to_bin(av[0]);

  blocks::Game game(ac, av);
  Game::instance(&game);
  game.start();

  return 0;
}
