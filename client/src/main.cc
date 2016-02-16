
#include "game.hh"

#include <iostream>
#include <string>

using namespace std;
using namespace blocks;


#include "duktape.hh"

int main(int ac, char **av)
{
  Game game(ac, av);

  DukTape duk;
  duk.eval_str("42;");
  cout << duk.stack->result_int() << endl;
  // cout << duk.stack->result_bool() << endl;
  // cout << duk.stack->get_safe_string(-1) << endl;
  // exit(0);

  game.start();
  return 0;
}
