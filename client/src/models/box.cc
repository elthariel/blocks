
#include "models/box.hh"

#include <array>

namespace blocks
{
  namespace models
  {
    Box::Box(float width, float height, float depth)
      : _w(width), _h(height), _d(depth)
    {
      _vxd = new GeomVertexData("cube", GeomVertexFormat::get_v3n3t2(), Geom::UH_static);
      _vertex = GeomVertexWriter(_vxd, "vertex");
      _normal = GeomVertexWriter(_vxd, "normal");
      _uvs = GeomVertexWriter(_vxd, "texcoord");

      _vxd->reserve_num_rows(8);
    }

    Box::Box(float size)
      : Box(size, size, size)
    {
    }

    PT(Geom) Box::make_geom()
    {
      PT(Geom) geom = new Geom(_vxd);
      PT(GeomTriangles) mesh;
      std::array<LVector3f, 6> normals = {
        LVector3f(-1.0, 0.0, 0.0), LVector3f(1.0, 0.0, 0.0),
        LVector3f(0.0, -1.0, 0.0), LVector3f(0.0, 1.0, 0.0),
        LVector3f(0.0, 0.0, -1.0), LVector3f(0.0, 0.0, 1.0)
      };

      float whd[3] = {_w, _h, _d};
      for (auto d = 0; d < 3; d++)
      {
        auto u = (d + 1) % 3;
        auto v = (d + 2) % 3;
        float dd[3] = {0, 0, 0};
        float du[3] = {0, 0, 0};
        float dv[3] = {0, 0, 0};
        dd[d] = whd[d];
        du[u] = whd[u];
        dv[v] = whd[v];

        auto face = 2 * d;
        _vertex.add_data3f(0,             0,             0);
        _vertex.add_data3f(du[0],         du[1],         du[2]);
        _vertex.add_data3f(du[0] + dv[0], du[1] + dv[1], du[2] + dv[2]);
        _vertex.add_data3f(        dv[0],         dv[1],         dv[2]);
        _uvs.add_data2f(0, 0);
        _uvs.add_data2f(whd[u], 0);
        _uvs.add_data2f(whd[u], whd[v]);
        _uvs.add_data2f(0,      whd[v]);
        for (auto i = 0; i < 4; i++)
          _normal.add_data3f(normals[face][0], normals[face][1], normals[face][2]);


        face++;
        _vertex.add_data3f(0             + dd[0], 0             + dd[1], 0             + dd[2]);
        _vertex.add_data3f(du[0]         + dd[0], du[1]         + dd[1], du[2]         + dd[2]);
        _vertex.add_data3f(du[0] + dv[0] + dd[0], du[1] + dv[1] + dd[1], du[2] + dv[2] + dd[2]);
        _vertex.add_data3f(        dv[0] + dd[0],         dv[1] + dd[1],         dv[2] + dd[2]);
        for (auto i = 0; i < 4; i++)
          _normal.add_data3f(normals[face][0], normals[face][1], normals[face][2]);
        _uvs.add_data2f(0, 0);
        _uvs.add_data2f(whd[u], 0);
        _uvs.add_data2f(whd[u], whd[v]);
        _uvs.add_data2f(0,      whd[v]);


        for(auto i = 0; i < 2; i++)
        {
          mesh = new GeomTriangles(Geom::UH_static);
          mesh->add_vertices(d * 8 + 4 * i,
                             d * 8 + 4 * i + 2,
                             d * 8 + 4 * i + 1);
          mesh->add_vertices(d * 8 + 4 * i,
                             d * 8 + 4 * i + 3,
                             d * 8 + 4 * i + 2);
          mesh->close_primitive();
          geom->add_primitive(mesh);
        }
      }

      return geom;
    }

    PT(GeomNode) Box::make(std::string name)
    {
      auto geom = make_geom();
      PT(GeomNode) node = new GeomNode(name);
      node->add_geom(geom);
      return node;
    }

    SkyBox::SkyBox(float size)
      : Box(size)
    {
    }

    PT(GeomNode) SkyBox::make(std::string name)
    {
      auto geom = make_geom();
      geom->doubleside_in_place();
      PT(GeomNode) node = new GeomNode(name);
      node->add_geom(geom);
      return node;
    }
  }
}
