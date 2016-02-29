
#include "session.hh"

using namespace std;

namespace blocks
{
  Session *Session::_instance = nullptr;

  Session::~Session()
  {
    for(auto iter: _storage)
      if (iter.second != nullptr)
        delete iter.second;
  }

  Session &Session::instance()
  {
    if (_instance == nullptr)
      _instance = new Session;

    return *_instance;
  }

  void Session::destroy()
  {
    if (_instance != nullptr)
    {
      delete _instance;
      _instance = nullptr;
    }
  }

}
