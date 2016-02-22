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

    uint16_t get_vertex_id(const common::cpos &p);
    void set_vertex_id(const common::cpos &p, uint16_t vxid);

    bool get_face_visible(const common::cpos &p, unsigned face);
    void set_face_visible(const common::cpos &p, unsigned face, bool visible);
    bool get_face_meshed(const common::cpos &p, unsigned face);
    void set_face_meshed(const common::cpos &p, unsigned face, bool meshed);
    auto &block_ids();

  protected:
    uint32_t &flags_at(const common::cpos &p);
    bool get_flag(const common::cpos &p, unsigned offset);
    void set_flag(const common::cpos &p, unsigned offset, bool set);

    array3<uint32_t> _flags;
    array3<int32_t> _vertices;
    std::list<int32_t> _block_ids;
  };

  class GreedyMesher{
  public:
    const static std::array<LVector3f, 6> normals;
    const static std::array<const common::cpos, 6> neighbors;

    GreedyMesher(common::Chunk::ptr);
    PT(GeomNode) mesh();
  protected:
    void compute_visible();
    bool need_mesh(const common::cpos &iter, int face_idx, uint16_t block_type);
    void mesh_face(common::cpos &iter, uint16_t block_type, int d, int u, int v, char front);
    void create_quad(unsigned char face,
                     bool front,
                     const common::cpos &base,
                     const common::cpos& du, const common::cpos& dv,
                     int w, int h = 1);

    void start_mesh();
    void finish_mesh(PT(GeomNode) geom_node, uint16_t block_type);

    MesherWorkBuffer _work;
    common::Chunk::ptr _chunk;

    // Panda Data
    size_t _vx_count = 0;
    size_t _face_count = 0;
    PT(GeomVertexData) _vxd;
    PT(GeomTriangles) _mesh;
    GeomVertexWriter _vertex, _normal, _uvs;
  };
}
