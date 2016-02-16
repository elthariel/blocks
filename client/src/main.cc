
#include "game.hh"

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace std;

int main(int ac, char **av)
{
  if (ac != 3)
  {
    std::cerr << "Usage : ./client IP PORT" << std::endl;
    return (0);
  }

  blocks::Game game(ac, av);
  game.start();

  return 0;
}

// #include "Client.hh"
// int			   main(int ac, char **av)
// {
//   if (ac != 3)
//   {
//     std::cerr << "Usage : ./client IP PORT" << std::endl;
//     return (0);
//   }
//   Client client(av[1], av[2]);
//   return (0);
// }
