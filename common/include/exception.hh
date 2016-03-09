#pragma once

#include <string>

namespace blocks
{
     class exception
     {
     public:
          exception(const std::string &msg) :_msg(msg) {}
          exception() :exception("Unknown exception") {}

          operator std::string() { return _msg; }

     protected:
          std::string _msg;
     };
};
