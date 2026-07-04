#pragma once
#include "geometry.hpp"
#include "image.hpp"
#include <array>
#include <optional>
#include <string>
#include <vector>

// A triangle mesh: flat arrays of unique positions/UVs/normals, plus one
// index triple per triangle corner into each array. Storing indices instead
// of duplicating full vertex data mirrors how OBJ files themselves work
// (separate v/vt/vn pools referenced by f lines).
//
// Faces are always triangles: loadObj fan-triangulates any face line with
// more than three vertices, and the procedural generators only ever emit
// triangles.
class Model {
public:
    static Model cube();                              // 8 verts, 12 triangles, face normals, planar UVs
    static Model sphere(int lat, int lon);             // UV-sphere; normals = normalized position
    static Model loadObj(const std::string& path);     // throws std::runtime_error if file won't open

    int   nfaces() const;
    Vec3f vert(int face, int nth) const;                // nth in {0,1,2}
    Vec2f uv(int face, int nth) const;
    Vec3f normal(int face, int nth) const;

    void  setDiffuse(const Image& tex);                 // stores a copy of a diffuse texture
    // Samples the diffuse texture at uv (v=1 is texture-top); with no texture
    // set, returns a procedural 8x8 checkerboard so lessons render something
    // meaningful before a real texture file exists.
    Color diffuse(Vec2f uv) const;

private:
    std::vector<Vec3f> verts_, norms_;
    std::vector<Vec2f> uvs_;
    // One index triple per face corner: faceV_[f][n] indexes verts_ for the
    // n-th corner of face f, and likewise faceUV_/faceN_ index uvs_/norms_.
    std::vector<std::array<int, 3>> faceV_, faceUV_, faceN_;
    std::optional<Image> diffuse_;
};
