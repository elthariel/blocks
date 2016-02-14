
#include "seed.hh"

#include <iostream>
#include <fstream>
using namespace std;

namespace blocks {
  template <uint16_t SIZE>
  seed_block<SIZE> seed_block<SIZE>::from_file(const std::string &path) {
    seed_block<SIZE> _seed;
    ifstream file ("/dev/urandom", ios::in|ios::binary);

    if (file.is_open()) {
      file.read(_seed._data, SIZE);
      file.close();
    } else {
      std::cerr << "Unable to read random data as a seed" << std::endl;
    }

    return _seed;
  }

  template <uint16_t SIZE>
  bool seed_block<SIZE>::save(const std::string &path) {
    ofstream file (path, ios::out|ios::binary);

    if (file.is_open()) {
      file.write(_data, SIZE);
      file.close();
      return true;
    } else {
      std::cerr << "Unable to save seed state" << std::endl;
      return false;
    }
  }
}
