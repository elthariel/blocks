
#include "entity.hh"
#include <utility>
#include <map>

namespace blocks
{
  class EntityMap
  {
  public:
    typedef ex::Entity::Id server_id_type;
    typedef ex::Entity::Id client_id_type;
    typedef std::pair<std::map<server_id_type, client_id_type>,
                      std::map<client_id_type, server_id_type>> container_type;

    void register_id(server_id_type s, client_id_type c);
    void invalidate_id(server_id_type);
    server_id_type server_id(client_id_type);
    client_id_type client_id(client_id_type);

  protected:
    container_type _map;
  };
}
