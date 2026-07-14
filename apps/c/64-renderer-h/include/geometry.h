#ifndef GEOMETRY_H
#define GEOMETRY_H

// C port of 53's geometry.hpp. The C-vs-C++ contrast is the lesson: C has no
// operator overloading and no templates, so the vector types are plain structs
// with x/y/z fields (not an indexable Vec<N,T>), and every operation is a
// NAMED function (vec3_add(a,b) instead of a + b). Values are passed and
// returned by value — these structs are small and copying is cheap and clear.

typedef struct { float x, y; }       Vec2f;
typedef struct { float x, y, z; }    Vec3f;
typedef struct { float x, y, z, w; } Vec4f;
typedef struct { float m[4][4]; }    Mat4;   // row-major, like 53's Matrix

Vec3f vec3_add(Vec3f a, Vec3f b);
Vec3f vec3_sub(Vec3f a, Vec3f b);
Vec3f vec3_scale(Vec3f v, float s);
float vec3_dot(Vec3f a, Vec3f b);
Vec3f vec3_cross(Vec3f a, Vec3f b);
float vec3_norm(Vec3f v);
Vec3f vec3_normalize(Vec3f v);

Vec4f vec4_embed(Vec3f v, float w);  // (x,y,z,w); w=1 for a point, 0 for a dir
Vec3f vec4_proj3(Vec4f v);           // perspective divide: (x/w, y/w, z/w)

Mat4  mat4_identity(void);
Mat4  mat4_mul(Mat4 a, Mat4 b);      // a*b (row-major)
Vec4f mat4_mulv(Mat4 a, Vec4f v);    // a * column-vector v

#endif // GEOMETRY_H
