
#include "common/position.hh"
#include "constants.hh"

#include <boost/format.hpp>

namespace blocks {
    namespace common {
      wpos::wpos(const class cid &_cid, const class cpos &_cpos)
      :pos(
        _cid.x() * consts::chunk_size + _cpos.x(),
        _cid.y() * consts::chunk_size + _cpos.y(),
        _cid.z() * consts::chunk_size + _cpos.z()
      ) { }

      cid wpos::cid() const {
        return blocks::common::cid(
          x() / consts::chunk_size,
          y() / consts::chunk_size,
          z() / consts::chunk_size
        );
      }

      cpos wpos::cpos() const {
        return blocks::common::cpos(
          x() % consts::chunk_size,
          y() % consts::chunk_size,
          z() % consts::chunk_size
        );
      }

      cpos::cpos(size_t idx)
      : pos(
        idx / (consts::chunk_size * consts::chunk_size),
        (idx / consts::chunk_size) % consts::chunk_size,
        idx % consts::chunk_size
      ) {}

      size_t cpos::to_idx() const {
        return x() * consts::chunk_size * consts::chunk_size +
          y() * consts::chunk_size +
          z();
      }

      bool cpos::valid() const {
        return (
          x() >= 0 && x() < consts::chunk_size &&
          y() >= 0 && y() < consts::chunk_size &&
          z() >= 0 && z() < consts::chunk_size
        );
      }

  }
}
