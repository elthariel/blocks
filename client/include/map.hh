#pragma once

#include "entity.hh"
#include "position.hh"

#include <map>

namespace blocks
{
  struct Map : public nocopy
  {
    typedef std::map<cid, ex::Entity> chunk_map;
    typedef std::map<cid, bool> loading_map;

    ex::Entity get(const cid &) const;
    void set(const cid &, ex::Entity);
    void remove(const cid &);
    bool exists(const cid &) const;

    bool loading(const cid &) const;
    void loading_started(const cid &);
    void loading_finished(const cid &);

    protected:
    chunk_map _map;
    loading_map _loading;
  };
}
