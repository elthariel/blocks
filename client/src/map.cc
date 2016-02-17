
#include "map.hh"

#include <iostream>

namespace blocks
{
  ex::Entity Map::get(const cid &_cid) const
  {
    if (_map.count(_cid))
      return _map.at(_cid);
    else
      // Invalid entity
      return ex::Entity();
  }

  void Map::set(const cid & _cid, ex::Entity _entity)
  {
    if (_map[_cid])
      std::cout << "Overriding a valid entity in map" << std::endl;

    _map[_cid] = _entity;
  }

  void Map::remove(const cid &_cid)
  {
    _map.erase(_cid);
  }

  bool Map::exists(const cid &_cid) const
  {
    if (_map.count(_cid))
      return _map.at(_cid).valid();
    else
      return false;
  }

  bool Map::loading(const cid &_cid) const
  {
    if (_loading.count(_cid))
      return _loading.at(_cid);
    else
      return false;
  }

  void Map::loading_started(const cid &_cid)
  {
    _loading[_cid] = true;
  }

  void Map::loading_finished(const cid &_cid)
  {
    _loading[_cid] = false;
  }
}
