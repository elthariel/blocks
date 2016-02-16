
#include "game.hh"

#include <iostream>
#include <string>
#include		   <boost/asio.hpp>
#include		   <boost/thread.hpp>
using namespace std;

// int main(int ac, char **av) {
//   cout << Power<4, 2>::value << endl;
// }

#include "duktape.hh"

int main(int ac, char **av)
{
  if (ac != 3)
  {
    std::cerr << "Usage : ./client IP PORT" << std::endl;
    return (0);
  }

  blocks::Game game(ac, av);

  blocks::DukTape duk;
  duk.eval_str("42;");
  cout << duk.stack->result_int() << endl;
  // cout << duk.stack->result_bool() << endl;
  // cout << duk.stack->get_safe_string(-1) << endl;
  // exit(0);

  game.start();
  return 0;
}
