
#include "game.hh"

#include <iostream>
#include <string>

using namespace std;
using namespace blocks;


#include "livescript.hh"

int main(int ac, char **av)
{
  //Game game(ac, av);

  DukTape ls;
  ls.eval("print('More blocks!');");
  exit(0);

  //game.start();
  return 0;
}
