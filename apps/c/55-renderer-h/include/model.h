#ifndef MODEL_H
#define MODEL_H

#include "geometry.h"
#include "image.h"

// C port of 53's model.hpp. Contrast with C++: no std::vector, so each attribute
// pool is a hand-grown dynamic array (malloc/realloc, freed by model_free), and
// no std::optional, so an optional texture is an Image plus a "has it?" flag.

typedef struct { int idx[3]; } Face;  // three indices, one per triangle corner

typedef struct {
    Vec3f* verts;  int nverts,  capverts;
    Vec3f* norms;  int nnorms,  capnorms;
    Vec2f* uvs;    int nuvs,    capuvs;
    // Parallel index arrays: faceV/faceUV/faceN all have nfaces entries.
    Face*  faceV;
    Face*  faceUV;
    Face*  faceN;
    int    nfaces, capfaces;

    Image  diffuse;    int has_diffuse;
    Image  normalmap;  int has_normalmap;
} Model;

Model model_cube(void);                 // 12 triangles, face normals, planar UVs
Model model_sphere(int lat, int lon);   // UV-sphere; normals = normalized position
int   model_load_obj(Model* out, const char* path);  // 0 ok, -1 fail; fills *out
void  model_free(Model* m);             // frees all pools + any textures

int   model_nfaces(const Model* m);
Vec3f model_vert(const Model* m, int face, int nth);
Vec2f model_uv(const Model* m, int face, int nth);
Vec3f model_normal(const Model* m, int face, int nth);

void  model_set_diffuse(Model* m, const Image* tex);    // stores a deep copy
Color model_diffuse(const Model* m, Vec2f uv);          // texture or checkerboard
void  model_set_normalmap(Model* m, const Image* nm);   // stores a deep copy
Vec3f model_normalmap(const Model* m, Vec2f uv);        // decoded normal or (0,0,1)

#endif // MODEL_H
