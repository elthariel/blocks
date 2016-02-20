
#include "mesher.hh"

#include <geomVertexFormat.h>
#include <iostream>

using namespace std;

namespace blocks {
  //
  // Work Buffer
  //

  MesherWorkBuffer::MesherWorkBuffer(size_t size)
  : _flags(boost::extents[size][size][size]),
    _vertices(boost::extents[size + 1][size + 1][size + 1])
  {
    for (size_t x = 0; x < size; ++x)
      for (size_t y = 0; y < size; ++y)
        for (size_t z = 0; z < size; ++z){
          _flags[x][y][z] = 0;
          _vertices[x][y][z] = -1;
        }
  }

  uint16_t MesherWorkBuffer::get_vertex_id(const common::cpos &p) {
    return _vertices[p.x()][p.y()][p.z()];
  }

  void MesherWorkBuffer::set_vertex_id(const common::cpos &p, uint16_t vxid) {
    _vertices[p.x()][p.y()][p.z()] = vxid;
  }

  bool MesherWorkBuffer::get_face_visible(const common::cpos &p, unsigned face) {
    return get_flag(p, face);
  }

  void MesherWorkBuffer::set_face_visible(const common::cpos &p, unsigned face, bool visible) {
    return set_flag(p, face, visible);
  }

  bool MesherWorkBuffer::get_face_meshed(const common::cpos &p, unsigned face) {
    return get_flag(p, face + 6);
  }

  void MesherWorkBuffer::set_face_meshed(const common::cpos &p, unsigned face, bool meshed) {
    return set_flag(p, face + 6, meshed);
  }

  uint16_t &MesherWorkBuffer::flags_at(const common::cpos &p) {
    return _flags[p.x()][p.y()][p.z()];
  }

  bool MesherWorkBuffer::get_flag(const common::cpos &p, unsigned offset) {
    return flags_at(p) & (1L << offset);
  }

  void MesherWorkBuffer::set_flag(const common::cpos &p, unsigned offset, bool set) {
    if (set)
    flags_at(p) |= 1L << offset;
    else
    flags_at(p) &= ~(1L << offset);
  }

  auto &MesherWorkBuffer::block_ids()
  {
    return _block_ids;
  }

  //
  // Greedy Mesher !
  //
  const std::array<LVector3f, 6> GreedyMesher::normals = {
    LVector3f(1.0, 0.0, 0.0), LVector3f(-1.0, 0.0, 0.0),
    LVector3f(0.0, 1.0, 0.0), LVector3f(0.0, -1.0, 0.0),
    LVector3f(0.0, 0.0, 1.0), LVector3f(0.0, 0.0, -1.0)
  };

  const std::array<const common::cpos, 6> GreedyMesher::neighbors = {
    common::cpos(1, 0, 0), common::cpos(-1, 0, 0),
    common::cpos(0, 1, 0), common::cpos(0, -1, 0),
    common::cpos(0, 0, 1), common::cpos(0, 0, -1)
  };

  GreedyMesher::GreedyMesher(common::Chunk::ptr c)
  : _chunk(c), _work(c->size())
  {
    compute_visible();
  }

  void GreedyMesher::compute_visible()
  {
    for (size_t idx = 0; idx < common::Chunk::flat_size(); ++idx)
    {
      bool visible_face = false;
      common::cpos pos(idx);

      if (_chunk->at(pos).air())
        continue;

      for (auto f = 0; f < neighbors.size(); ++f)
      {
        common::cpos pos_nb = pos + neighbors[f];
        if (!pos_nb.valid() || _chunk->at(pos_nb).air()
            || _chunk->at(pos_nb).transparent())
        {
          _work.set_face_visible(pos, f, true);
          visible_face = true;
          continue;
        }
      }
      if (visible_face)
        _work.block_ids().push_back(_chunk->at(pos).id());
    }
    _work.block_ids().sort();
    _work.block_ids().unique();
  }

  void GreedyMesher::start_mesh()
  {
    _vxd = new GeomVertexData(_chunk->id(), GeomVertexFormat::get_v3n3t2(), Geom::UH_static);
    _vertex = GeomVertexWriter(_vxd, "vertex");
    _normal = GeomVertexWriter(_vxd, "normal");
    _uvs = GeomVertexWriter(_vxd, "texcoord");
    _mesh = new GeomTriangles(Geom::UH_static);
    _vx_count = 0;

    _vxd->reserve_num_rows(1000);
  }

  void GreedyMesher::finish_mesh(PT(GeomNode) geom_node, uint16_t block_id)
  {
    auto name = std::string("blockid:") + std::to_string(block_id);
    PT(GeomNode) subnode = new GeomNode(name);
    // Now we've filled our VertexData object, let's create a primitive and everything
    _mesh->close_primitive();
    PT(Geom) geom = new Geom(_vxd);
    geom->add_primitive(_mesh);

    subnode->add_geom(geom);
    subnode->set_tag("block_id", std::to_string(block_id));

    geom_node->add_child(subnode);
  }

  void GreedyMesher::create_quad(unsigned char face,
                                 bool front,
                                 const common::cpos &base,
                                 const common::cpos& du, const common::cpos& dv,
                                 int w, int h)
  {
    int64_t quads[4][3] = {
      { base[0],             base[1],             base[2]             },
      { base[0]+du[0],       base[1]+du[1],       base[2]+du[2]       },
      { base[0]+du[0]+dv[0], base[1]+du[1]+dv[1], base[2]+du[2]+dv[2] },
      { base[0]+dv[0],       base[1]+dv[1],       base[2]+dv[2]       }
    };

    // cout << "Creating quad: " << std::string(base) << " -- "
    //      << w << ":"  << h << ":"
    //      << endl;

    for(auto i = 0; i < 4; ++i)
    {
      _vertex.add_data3f(quads[i][0], quads[i][1], quads[i][2]);
      _normal.add_data3f(normals[face][0], normals[face][1], normals[face][2]);
    }

    _uvs.add_data2f(0, 0);
    _uvs.add_data2f(0, h);
    _uvs.add_data2f(w, h);
    _uvs.add_data2f(w, 0);

    if (front)
    {
      _mesh->add_vertices(_vx_count, _vx_count + 2, _vx_count + 1);
      _mesh->add_vertices(_vx_count, _vx_count + 3, _vx_count + 2);
    }
    else
    {
      _mesh->add_vertices(_vx_count, _vx_count + 1, _vx_count + 2);
      _mesh->add_vertices(_vx_count, _vx_count + 2, _vx_count + 3);
    }

    _vx_count += 4;
  }

  bool GreedyMesher::need_mesh(const common::cpos &pos, int face_idx, uint16_t block_id)
  {
    auto visible = _work.get_face_visible(pos, face_idx);
    auto meshed = _work.get_face_meshed(pos, face_idx);
    return visible && !meshed && _chunk->at(pos).id() == block_id;
  }

  void GreedyMesher::mesh_face(common::cpos &iter,
                               uint16_t block_id,
                               int d, int u, int v,
                               char front)
  {
    auto face_idx = d * 2 + front;
    auto normal = normals[face_idx];

    // We then iterate from 0 to size on 3 axis, using the previously
    // defined dimension index to compute the position we're at in the
    // direction we're iterating

    for (auto i = 0; i < consts::chunk_size; i++)
    {
      iter[d] = i;
      for (auto j = 0; j < consts::chunk_size; j++)
      {
        iter[u] = j;
        for (auto k = 0; k < consts::chunk_size; k++)
        {
          iter[v] = k;
          // iter now have the chunk position of the block we're looking at
          // and will progress in the correct order for the direction we're
          // iterating.
          common::cpos iter2(iter), du, dv;
          auto w = 0, h = 0;

          // std::cout << std::string(iter) << std::endl;

          // Here we look for adjacent faces to merge
          // FIXME: We only do it in one direction now
          for(auto k2 = k; k2 < consts::chunk_size; ++k2)
          {
            iter2[v] = k2;

            if (need_mesh(iter2, face_idx, block_id))
            {
              _work.set_face_meshed(iter2, face_idx, true);
              dv[v]++; w++;
            }
            else
              break;
          }

          // There's a quad to mesh
          if (dv[v])
          {
            du[u]++;
            h++;
            // Trying to find if we can aggregate a quad of width 'w' on top of the one
            // we just found.
            for(auto j2 = j + 1; j2 < consts::chunk_size; ++j2)
            {
              iter2[u] = j2;

              // Ensure all the faces form k to k + w are visible and need mesh
              bool line_need_mesh = true;
              for(auto k2 = k; k2 < k + w; ++k2)
              {
                iter2[v] = k2;
                line_need_mesh = line_need_mesh && need_mesh(iter2, face_idx, block_id);
                if (!line_need_mesh)
                  break;
              }
              // Leave if they don't
              if (!line_need_mesh)
                break;

              // We can aggreage the 'j2' line
              for(auto k2 = k; k2 < k + w; ++k2)
              {
                iter2[v] = k2;
                _work.set_face_meshed(iter2, face_idx, true);
              }

              du[u]++;
              h++;
            }

            common::cpos base(iter);
            if (!front)
              base[d]++;

            create_quad(face_idx, front, base, du, dv, w, h);
          }
        }
      }
    }
  }

  PT(GeomNode) GreedyMesher::mesh() {
    PT(GeomNode) node = new GeomNode(std::string("chunk:") + std::string(_chunk->id()));
    common::cpos iter;

    // Iterator over the list of different block ideas we found during the visibility tests
    for(auto &block_id: _work.block_ids())
    {
      start_mesh();
      // Iterate all 3 dimension. d, u, and v will be the index of the dimensions
      // we iterate. This will allow us to iterate on x/y/z then y/z/x then z/y/x.
      for (auto d = 0; d < 3; ++d)
      {
        unsigned char u, v;
        u = (d + 1) % 3;
        v = (d + 2) % 3;

        // Then for each dimension, we iterate the front and back face
        for (auto front = 0; front < 2; ++front)
        {
          mesh_face(iter, block_id, d, u, v, front);
        }
      }
      finish_mesh(node, block_id);
    }

    return node;
  }
}
