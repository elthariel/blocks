
#include "map.hh"

#include <iostream>

namespace blocks
{
  ex::Entity Map::get(const common::cid &_cid) const
  {
    if (_map.count(_cid))
      return _map.at(_cid);
    else
      // Invalid entity
      return ex::Entity();
  }

  void Map::set(const common::cid & _cid, ex::Entity _entity)
  {
    if (_map[_cid])
      std::cout << "Overriding a valid entity in map" << std::endl;

    _map[_cid] = _entity;
  }

  void Map::remove(const common::cid &_cid)
  {
    _map.erase(_cid);
  }

  bool Map::exists(const common::cid &_cid) const
  {
    if (_map.count(_cid))
      return _map.at(_cid).valid();
    else
      return false;
  }

  bool Map::loading(const common::cid &_cid) const
  {
    if (_loading.count(_cid))
      return _loading.at(_cid);
    else
      return false;
  }

  void Map::loading_started(const common::cid &_cid)
  {
    _loading[_cid] = true;
  }

  void Map::loading_finished(const common::cid &_cid)
  {
    _loading[_cid] = false;
  }
}
