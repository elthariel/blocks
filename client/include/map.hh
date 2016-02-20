#pragma once

#include "entity.hh"
#include "common/position.hh"

#include <map>

namespace blocks
{
  struct Map : public nocopy
  {
    typedef std::map<common::cid, ex::Entity> chunk_map;
    typedef std::map<common::cid, bool> loading_map;

    ex::Entity get(const common::cid &) const;
    void set(const common::cid &, ex::Entity);
    void remove(const common::cid &);
    bool exists(const common::cid &) const;

    bool loading(const common::cid &) const;
    void loading_started(const common::cid &);
    void loading_finished(const common::cid &);

    protected:
    chunk_map _map;
    loading_map _loading;
  };
}
