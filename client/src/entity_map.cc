
#include "entity_map.hh"

namespace blocks
{
  void EntityMap::register_id(server_id_type serv, client_id_type cli)
  {
    _map.first[serv] = cli;
    _map.second[cli] = serv;
  }

  void EntityMap::invalidate_id(server_id_type serv)
  {
    if (_map.first.count(serv))
    {
      client_id_type cli = _map.first[serv];

      _map.first.erase(serv);
      _map.second.erase(cli);
    }
  }

  EntityMap::server_id_type EntityMap::server_id(client_id_type cli)
  {
    if (_map.first.count(cli))
      return _map.first[cli];
    else
      return client_id_type();
  }

  EntityMap::client_id_type EntityMap::client_id(server_id_type serv)
  {
    if (_map.second.count(serv))
      return _map.second[serv];
    else
      return server_id_type();
  }
}
