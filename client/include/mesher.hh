#pragma once

#include "common/chunk.hh"
#include "multi_array.hh"

#include <array>
#include <list>
#include <geomNode.h>
#include <geomVertexData.h>
#include <geomVertexWriter.h>
#include <geomTriangles.h>
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
    auto &block_ids();

  protected:
    uint16_t &flags_at(const cpos &p);
    bool get_flag(const cpos &p, unsigned offset);
    void set_flag(const cpos &p, unsigned offset, bool set);

    array3<uint16_t> _flags;
    array3<int16_t> _vertices;
    std::list<int16_t> _block_ids;
  };

  class GreedyMesher{
  public:
    const static std::array<LVector3f, 6> normals;
    const static std::array<const cpos, 6> neighbors;

    GreedyMesher(Chunk::ptr);
    PT(GeomNode) mesh();
  protected:
    void compute_visible();
    bool need_mesh(const cpos &iter, int face_idx, uint16_t block_type);
    void mesh_face(cpos &iter, uint16_t block_type, int d, int u, int v, char front);
    void create_quad(unsigned char face,
                     bool front,
                     const cpos &base,
                     const cpos& du, const cpos& dv,
                     int w, int h = 1);

    void start_mesh();
    void finish_mesh(PT(GeomNode) geom_node, uint16_t block_type);

    MesherWorkBuffer _work;
    Chunk::ptr _chunk;

    // Panda Data
    size_t _vx_count = 0;
    PT(GeomVertexData) _vxd;
    PT(GeomTriangles) _mesh;
    GeomVertexWriter _vertex, _normal, _uvs;
  };
}
