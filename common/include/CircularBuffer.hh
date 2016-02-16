#pragma once

#include		  <iostream>
#include		  <vector>
#include		  <boost/circular_buffer.hpp>
#include		  <boost/thread/mutex.hpp>

class			  CircularBuffer
{
public:
  CircularBuffer();

public:
  void			  add(uint8_t *, size_t);
  uint8_t 		  *get(size_t);

private:
  boost::circular_buffer<uint8_t> _cb;
  boost::mutex    _mutex;
};
