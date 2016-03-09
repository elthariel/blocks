#include		  "CircularBuffer.hh"

CircularBuffer::CircularBuffer() : _cb(4096) {}

void CircularBuffer::add(uint8_t *data, size_t size)
{
  boost::mutex::scoped_lock s(_mutex);

  for (size_t i = 0; i < size; i++) _cb.push_back(data[i]);
}

uint8_t *CircularBuffer::get(size_t size)
{
  boost::mutex::scoped_lock s(_mutex);
  std::vector<uint8_t> res;

  if (_cb.size() < size) return nullptr;

  while (size-- && _cb.size()) {
    res.push_back(_cb.front());
    _cb.pop_front();
  }

  auto buff = new uint8_t[res.size()];

  std::memcpy(buff, res.data(), res.size());
  return (buff);
}
