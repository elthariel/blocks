
#include "mesher.hh"

namespace blocks {
  //
  // Work Buffer
  //

  MesherWorkBuffer::MesherWorkBuffer(size_t size)
  : _flags(boost::extents[size][size][size]),
    _vertices(boost::extents[size][size][size])
  {
    for (size_t x = 0; x < size; ++x)
      for (size_t y = 0; y < size; ++y)
        for (size_t z = 0; z < size; ++z){
          _flags[x][y][z] = 0;
          _vertices[x][y][z] = -1;
        }
  }

  uint16_t MesherWorkBuffer::get_vertex_id(const cpos &p) {
    return _vertices[p.x()][p.y()][p.z()];
  }

  void MesherWorkBuffer::set_vertex_id(const cpos &p, uint16_t vxid) {
    _vertices[p.x()][p.y()][p.z()] = vxid;
  }

  bool MesherWorkBuffer::get_face_visible(const cpos &p, unsigned face) {
    return get_flag(p, face);
  }

  void MesherWorkBuffer::set_face_visible(const cpos &p, unsigned face, bool visible) {
    return set_flag(p, face, visible);
  }

  bool MesherWorkBuffer::get_face_meshed(const cpos &p, unsigned face) {
    return get_flag(p, face + 6);
  }

  void MesherWorkBuffer::set_face_meshed(const cpos &p, unsigned face, bool meshed) {
    return set_flag(p, face + 6, meshed);
  }

  uint16_t &MesherWorkBuffer::flags_at(const cpos &p) {
    return _flags[p.x()][p.y()][p.z()];
  }

  bool MesherWorkBuffer::get_flag(const cpos &p, unsigned offset) {
    return flags_at(p) & (1L << offset);
  }

  void MesherWorkBuffer::set_flag(const cpos &p, unsigned offset, bool set) {
    if (set)
    flags_at(p) |= 1L << offset;
    else
    flags_at(p) &= ~(1L << offset);
  }

  //
  // Greedy Mesher !
  //
  const std::array<LVector3f, 6> GreedyMesher::normals = {
    LVector3f(1.0, 0.0, 0.0), LVector3f(-1.0, 0.0, 0.0),
    LVector3f(0.0, 1.0, 0.0), LVector3f(0.0, -1.0, 0.0),
    LVector3f(0.0, 0.0, 1.0), LVector3f(0.0, 0.0, -1.0)
  };

  const std::array<const cpos, 6> GreedyMesher::neighbors = {
    cpos(1, 0, 0), cpos(-1, 0, 0),
    cpos(0, 1, 0), cpos(0, -1, 0),
    cpos(0, 0, 1), cpos(0, 0, -1)
  };

  GreedyMesher::GreedyMesher(Chunk::ptr c)
  : _chunk(c), _work(c->size())
  {
    compute_visible();
  }

  void GreedyMesher::compute_visible() {
    for (size_t idx = 0; idx < Chunk::size(); ++idx) {
      cpos pos(idx);
      for(auto f = 0; f < neighbors.size(); ++f) {
        cpos pos_nb = pos + neighbors[f];
        if (!pos_nb.valid()) {
          _work.set_face_visible(pos, f, true);
          continue;
        } else if ((*_chunk)[pos].air() || (*_chunk)[pos].transparent()) {
          _work.set_face_visible(pos, f, true);
          continue;
        }
      }
    }
  }

  PT(GeomNode) GreedyMesher::mesh() {

  }
}
