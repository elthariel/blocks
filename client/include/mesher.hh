#pragma once

#include "chunk.hh"
#include "multi_array.hh"

#include <array>
#include <geomNode.h>
#include <lvector3.h>

namespace blocks {
  class MesherWorkBuffer {
  public:
    MesherWorkBuffer(size_t size);

    uint16_t get_vertex_id(const cpos &p);
    void set_vertex_id(const cpos &p, uint16_t vxid);

    bool get_face_visible(const cpos &p, unsigned face);
    void set_face_visible(const cpos &p, unsigned face, bool visible);
    bool get_face_meshed(const cpos &p, unsigned face);
    void set_face_meshed(const cpos &p, unsigned face, bool meshed);

  protected:
    uint16_t &flags_at(const cpos &p);
    bool get_flag(const cpos &p, unsigned offset);
    void set_flag(const cpos &p, unsigned offset, bool set);

    array3<uint16_t> _flags;
    array3<int16_t> _vertices;
  };

  class GreedyMesher{
  public:
    const static std::array<LVector3f, 6> normals;
    const static std::array<const cpos, 6> neighbors;

    GreedyMesher(Chunk::ptr);
    PT(GeomNode) mesh();
  protected:
    void compute_visible();

    MesherWorkBuffer _work;
    Chunk::ptr _chunk;
  };
}
