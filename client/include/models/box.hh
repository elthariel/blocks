#pragma once

#include "geomNode.h"
#include "geomVertexData.h"
#include "geomTriangles.h"
#include "geomVertexWriter.h"

#include <string>

namespace blocks
{
  namespace models
  {
    class Box
    {
    public:
      Box(float width, float height, float depth);
      Box(float size);
      PT(Geom) make_geom();
      PT(GeomNode) make(std::string name);
    protected:
      Box();

      float _w, _h, _d;
      PT(GeomVertexData) _vxd;
      PT(GeomTriangles) _mesh;
      GeomVertexWriter _vertex, _normal, _uvs;
    };

    class SkyBox : public Box
    {
    public:
      SkyBox(float size);
      PT(GeomNode) make(std::string name);

    protected:
      // const std::string &_texture_path;
    };
  }
}
