#ifndef SHADER_H
#define SHADER_H

#include "geometry.h"
#include "image.h"
#include "model.h"

// C port of 53's shader.hpp. Contrast with C++: there is no virtual IShader, so
// polymorphism is done by hand — a Shader is a struct of FUNCTION POINTERS plus
// a `state` blob. Each concrete shader has its own state struct (holding the
// const uniforms + the per-vertex "varyings"); its vertex/fragment functions
// cast `sh->state` back to that type. The caller owns the state (on its stack),
// so there is nothing to free.

typedef struct Shader Shader;
struct Shader {
    void* state;
    Vec4f (*vertex)(Shader* sh, int face, int nth);      // -> clip space; stashes varyings
    int   (*fragment)(Shader* sh, Vec3f bary, Color* out);  // return 1 to DISCARD
};

// Flat: one constant intensity per face (averaged vertex normals). Facet edges.
typedef struct {
    const Model* model; Mat4 mvp; Vec3f light;
    Vec2f varyingUV[3];
    Vec3f normalSum;
    float intensity;
} FlatState;

// Gouraud: per-vertex intensity, interpolated across the face.
typedef struct {
    const Model* model; Mat4 mvp; Vec3f light;
    Vec2f varyingUV[3];
    float varyingIntensity[3];
} GouraudState;

// Phong: per-fragment interpolated normal, lit per pixel.
typedef struct {
    const Model* model; Mat4 mvp; Vec3f light;
    Vec2f varyingUV[3];
    Vec3f varyingNormal[3];
} PhongState;

// Normal-mapped: like Phong, but the per-fragment normal comes from a texture
// transformed through a per-triangle TBN basis.
typedef struct {
    const Model* model; Mat4 mvp; Vec3f light;
    Vec2f varyingUV[3];
    Vec3f varyingNormal[3];
    Vec3f varyingPos[3];
} NormalMapState;

// Each initializes *s and returns a Shader wired to it.
Shader flat_shader(FlatState* s, const Model* model, Mat4 mvp, Vec3f light);
Shader gouraud_shader(GouraudState* s, const Model* model, Mat4 mvp, Vec3f light);
Shader phong_shader(PhongState* s, const Model* model, Mat4 mvp, Vec3f light);
Shader normalmap_shader(NormalMapState* s, const Model* model, Mat4 mvp, Vec3f light);

#endif // SHADER_H
