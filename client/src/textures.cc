#include "textures.hh"

#include <iostream>

using namespace std;

namespace blocks
{
  Textures::Textures()
    : _mrb(), _mats(_mrb)
  {
    _parsed = parse_dsl();
  }

  bool Textures::parse_dsl()
  {
    _mrb.eval_file("client/materials_dsl.rb");
    if (_mrb.exception())
    {
      cerr << "Exception while evaluating materials_dsl.rb" << endl;
      return false;
    }

    _mats = _mrb.eval_file("client/materials.rb");
    if (_mrb.exception())
    {
      cerr << "Exception while evaluating materials.rb" << endl;
      return false;
    }

    return true;
  }

}
