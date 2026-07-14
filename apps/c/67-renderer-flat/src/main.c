// 65-renderer-flat — the ENTIRE tinyrenderer pipeline in one C translation unit.
//
// A mechanical flatten of the multi-file 64-renderer-h: identical renderer,
// single file. See apps/c/65-renderer-flat/README.md for the layout contrast
// and references/tinyrenderer-pipeline.md for the theory. Differences from 55
// are organizational only: include guards and cross-file #include "..." lines
// are gone, and every standard-library header is hoisted (deduplicated) below.
// Order: all declarations (geometry, image, model, shader, gl) then all
// definitions. The file-local helpers keep the `static` they had in 55.

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===================== geometry.h =====================

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


// ===================== image.h =====================

// C port of 53's image.hpp. Contrast with C++: the framebuffer is a struct with
// a hand-`malloc`'d pixel buffer (no std::vector, no RAII) — the owner must call
// img_free (see main.c / model.c). PPM I/O returns status codes (0 ok, -1 fail)
// instead of throwing, because C has no exceptions.

typedef struct { unsigned char r, g, b; } Color;

typedef struct {
    int    w, h;
    Color* px;   // malloc'd w*h, row-major, origin top-left; NULL if unallocated
} Image;

Image img_create(int w, int h);   // zero-filled; px==NULL on allocation failure
void  img_free(Image* img);       // frees px, sets it NULL (safe to call twice)

void  img_set(Image* img, int x, int y, Color c);  // ignores out-of-range writes
Color img_get(const Image* img, int x, int y);     // clamps to edge (safe sampling)

int   img_write_ppm(const Image* img, const char* path);  // 0 ok, -1 fail
int   img_read_ppm(Image* out, const char* path);         // 0 ok, -1 fail; fills *out


// ===================== model.h =====================


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


// ===================== shader.h (struct Shader before gl_triangle uses it) =====================


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


// ===================== gl.h =====================


// C port of 53's gl.hpp. Same pipeline helpers as named functions; the z-buffer
// is a plain float* the caller owns. Conventions match 53 exactly: the viewport
// folds the single Y-flip; depth is [0,255] with LARGER z = nearer; the z-test
// is `z > zbuf`.


Mat4  gl_viewport(int x, int y, int w, int h);  // NDC -> screen, depth 0..255 (Y-flipped)
Mat4  gl_projection(float coeff);               // identity + m[3][2]=coeff
Mat4  gl_lookat(Vec3f eye, Vec3f center, Vec3f up);

void  gl_line(int x0, int y0, int x1, int y1, Image* img, Color c);  // Bresenham
Vec3f gl_barycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p);

// Rasterize a triangle whose corners are in CLIP space (from Shader.vertex):
// does the w-divide + viewport, scans the bounding box, z-tests, and calls
// sh->fragment per kept pixel. zbuf is img->w*img->h floats.
void  gl_triangle(const Vec4f clip[3], Shader* sh, Image* img, float* zbuf);


// ===================== geometry.c =====================

Vec3f vec3_add(Vec3f a, Vec3f b)   { return (Vec3f){a.x + b.x, a.y + b.y, a.z + b.z}; }
Vec3f vec3_sub(Vec3f a, Vec3f b)   { return (Vec3f){a.x - b.x, a.y - b.y, a.z - b.z}; }
Vec3f vec3_scale(Vec3f v, float s) { return (Vec3f){v.x * s, v.y * s, v.z * s}; }
float vec3_dot(Vec3f a, Vec3f b)   { return a.x * b.x + a.y * b.y + a.z * b.z; }

Vec3f vec3_cross(Vec3f a, Vec3f b) {
    return (Vec3f){a.y * b.z - a.z * b.y,
                   a.z * b.x - a.x * b.z,
                   a.x * b.y - a.y * b.x};
}

float vec3_norm(Vec3f v)      { return sqrtf(vec3_dot(v, v)); }
Vec3f vec3_normalize(Vec3f v) { return vec3_scale(v, 1.0f / vec3_norm(v)); }

Vec4f vec4_embed(Vec3f v, float w) { return (Vec4f){v.x, v.y, v.z, w}; }
Vec3f vec4_proj3(Vec4f v)          { return (Vec3f){v.x / v.w, v.y / v.w, v.z / v.w}; }

Mat4 mat4_identity(void) {
    Mat4 r = {{{0}}};
    for (int i = 0; i < 4; ++i) r.m[i][i] = 1.0f;
    return r;
}

Mat4 mat4_mul(Mat4 a, Mat4 b) {
    Mat4 r = {{{0}}};
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) sum += a.m[i][k] * b.m[k][j];
            r.m[i][j] = sum;
        }
    return r;
}

Vec4f mat4_mulv(Mat4 a, Vec4f v) {
    float in[4]  = {v.x, v.y, v.z, v.w};
    float out[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; ++i) {
        float sum = 0.0f;
        for (int k = 0; k < 4; ++k) sum += a.m[i][k] * in[k];
        out[i] = sum;
    }
    return (Vec4f){out[0], out[1], out[2], out[3]};
}

// ===================== image.c =====================

Image img_create(int w, int h) {
    Image img;
    img.w = w;
    img.h = h;
    // calloc zero-fills -> a black framebuffer. On failure px stays NULL and
    // callers (img_set/img_get) tolerate it; the owner still calls img_free.
    img.px = (w > 0 && h > 0) ? (Color*)calloc((size_t)w * (size_t)h, sizeof(Color)) : NULL;
    return img;
}

void img_free(Image* img) {
    if (!img) return;
    free(img->px);
    img->px = NULL;
}

void img_set(Image* img, int x, int y, Color c) {
    if (!img->px || x < 0 || x >= img->w || y < 0 || y >= img->h) return;
    img->px[(size_t)y * (size_t)img->w + (size_t)x] = c;
}

Color img_get(const Image* img, int x, int y) {
    if (!img->px) return (Color){0, 0, 0};
    int cx = x < 0 ? 0 : (x >= img->w ? img->w - 1 : x);
    int cy = y < 0 ? 0 : (y >= img->h ? img->h - 1 : y);
    return img->px[(size_t)cy * (size_t)img->w + (size_t)cx];
}

int img_write_ppm(const Image* img, const char* path) {
    FILE* f = fopen(path, "wb");
    if (!f) return -1;
    fprintf(f, "P6\n%d %d\n255\n", img->w, img->h);
    size_t n = (size_t)img->w * (size_t)img->h;
    for (size_t i = 0; i < n; ++i) {
        Color c = img->px[i];
        unsigned char rgb[3] = {c.r, c.g, c.b};
        if (fwrite(rgb, 1, 3, f) != 3) { fclose(f); return -1; }
    }
    fclose(f);
    return 0;
}

int img_read_ppm(Image* out, const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return -1;

    char magic[3] = {0};
    int w = 0, h = 0, maxval = 0;
    if (fscanf(f, "%2s %d %d %d", magic, &w, &h, &maxval) != 4 ||
        magic[0] != 'P' || magic[1] != '6' || w <= 0 || h <= 0 || maxval != 255) {
        fclose(f);
        return -1;
    }
    fgetc(f);  // the single whitespace byte between the header and pixel data

    Image img = img_create(w, h);
    if (!img.px) { fclose(f); return -1; }
    size_t n = (size_t)w * (size_t)h;
    if (fread(img.px, 3, n, f) != n) {  // Color is 3 tightly-packed bytes
        img_free(&img);
        fclose(f);
        return -1;
    }
    fclose(f);
    *out = img;
    return 0;
}

// ===================== model.c =====================

// A dynamic-array push: double the capacity when full, then append. This is the
// C stand-in for std::vector::push_back. ponytail: on realloc failure the old
// block leaks and the next write crashes — fine for the bounded inputs here
// (procedural meshes + well-formed .obj); add a checked grow if that changes.
#define PUSH(arr, n, cap, val)                                          \
    do {                                                               \
        if ((n) >= (cap)) {                                            \
            int nc_ = (cap) ? (cap) * 2 : 8;                          \
            (arr) = realloc((arr), (size_t)nc_ * sizeof(*(arr)));      \
            (cap) = nc_;                                               \
        }                                                             \
        (arr)[(n)++] = (val);                                         \
    } while (0)

static Model model_new(void) {
    Model m;
    memset(&m, 0, sizeof m);
    return m;
}

// Append one triangle: its vertex/uv/normal index triples grow in lockstep.
static void push_face(Model* m, Face fv, Face fuv, Face fn) {
    if (m->nfaces >= m->capfaces) {
        int nc = m->capfaces ? m->capfaces * 2 : 8;
        m->faceV  = realloc(m->faceV,  (size_t)nc * sizeof(Face));
        m->faceUV = realloc(m->faceUV, (size_t)nc * sizeof(Face));
        m->faceN  = realloc(m->faceN,  (size_t)nc * sizeof(Face));
        m->capfaces = nc;
    }
    m->faceV[m->nfaces]  = fv;
    m->faceUV[m->nfaces] = fuv;
    m->faceN[m->nfaces]  = fn;
    m->nfaces++;
}

static Image img_copy(const Image* src) {
    Image d = img_create(src->w, src->h);
    if (d.px && src->px)
        memcpy(d.px, src->px, (size_t)src->w * (size_t)src->h * sizeof(Color));
    return d;
}

// Split quad (a,b,c,d) — vertex indices, CCW from outside — into two triangles
// sharing normal index n and the shared planar-UV rectangle corners.
static void push_quad(Model* m, int a, int b, int c, int d, int n) {
    Face fn = {{n, n, n}};
    push_face(m, (Face){{a, b, c}}, (Face){{0, 1, 2}}, fn);
    push_face(m, (Face){{a, c, d}}, (Face){{0, 2, 3}}, fn);
}

Model model_cube(void) {
    Model m = model_new();
    Vec3f v[8] = {
        {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
        {-1, -1, 1},  {1, -1, 1},  {1, 1, 1},  {-1, 1, 1},
    };
    for (int i = 0; i < 8; ++i) PUSH(m.verts, m.nverts, m.capverts, v[i]);
    // Outward normals in face order: -Z, +Z, +X, -X, +Y, -Y.
    Vec3f nn[6] = {
        {0, 0, -1}, {0, 0, 1}, {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0},
    };
    for (int i = 0; i < 6; ++i) PUSH(m.norms, m.nnorms, m.capnorms, nn[i]);
    Vec2f uv[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    for (int i = 0; i < 4; ++i) PUSH(m.uvs, m.nuvs, m.capuvs, uv[i]);

    push_quad(&m, 0, 3, 2, 1, 0);  // -Z
    push_quad(&m, 4, 5, 6, 7, 1);  // +Z
    push_quad(&m, 1, 2, 6, 5, 2);  // +X
    push_quad(&m, 0, 4, 7, 3, 3);  // -X
    push_quad(&m, 2, 3, 7, 6, 4);  // +Y
    push_quad(&m, 1, 5, 4, 0, 5);  // -Y
    return m;
}

Model model_sphere(int lat, int lon) {
    Model m = model_new();
    const float pi = 3.14159265358979323846f;

    // A (lat+1) x (lon+1) vertex grid, pole to pole and around the equator; the
    // seam column is duplicated (same position, different UV) so texture coords
    // wrap cleanly.
    for (int i = 0; i <= lat; ++i) {
        float v = (float)i / (float)lat;
        float theta = v * pi;
        float y = cosf(theta);
        float r = sinf(theta);
        for (int j = 0; j <= lon; ++j) {
            float u = (float)j / (float)lon;
            float phi = u * 2.0f * pi;
            Vec3f pos = {r * cosf(phi), y, r * sinf(phi)};
            PUSH(m.verts, m.nverts, m.capverts, pos);
            PUSH(m.norms, m.nnorms, m.capnorms, vec3_normalize(pos));
            PUSH(m.uvs, m.nuvs, m.capuvs, ((Vec2f){u, v}));
        }
    }

    int stride = lon + 1;
    for (int i = 0; i < lat; ++i) {
        for (int j = 0; j < lon; ++j) {
            int a = i * stride + j;
            int b = i * stride + (j + 1);
            int c = (i + 1) * stride + (j + 1);
            int d = (i + 1) * stride + j;
            push_face(&m, (Face){{a, b, c}}, (Face){{a, b, c}}, (Face){{a, b, c}});
            push_face(&m, (Face){{a, c, d}}, (Face){{a, c, d}}, (Face){{a, c, d}});
        }
    }
    return m;
}

// Parse one OBJ face token ("v", "v/vt", "v/vt/vn", or "v//vn") into 0-based
// indices; missing vt/vn default to 0 (mirrors 53's parseFaceVertex).
static void parse_face_vertex(const char* tok, int* vi, int* ui, int* ni) {
    int a = 0, b = 0, c = 0;
    *vi = 0; *ui = 0; *ni = 0;
    if (sscanf(tok, "%d/%d/%d", &a, &b, &c) == 3)      { *vi = a - 1; *ui = b - 1; *ni = c - 1; }
    else if (sscanf(tok, "%d//%d", &a, &c) == 2)       { *vi = a - 1; *ni = c - 1; }
    else if (sscanf(tok, "%d/%d", &a, &b) == 2)        { *vi = a - 1; *ui = b - 1; }
    else if (sscanf(tok, "%d", &a) == 1)               { *vi = a - 1; }
}

int model_load_obj(Model* out, const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) return -1;

    Model m = model_new();
    char line[1024];
    while (fgets(line, sizeof line, f)) {
        if (line[0] == 'v' && line[1] == ' ') {
            Vec3f v;
            if (sscanf(line + 2, "%f %f %f", &v.x, &v.y, &v.z) == 3)
                PUSH(m.verts, m.nverts, m.capverts, v);
        } else if (line[0] == 'v' && line[1] == 't') {
            Vec2f t;
            if (sscanf(line + 3, "%f %f", &t.x, &t.y) == 2)
                PUSH(m.uvs, m.nuvs, m.capuvs, t);
        } else if (line[0] == 'v' && line[1] == 'n') {
            Vec3f n;
            if (sscanf(line + 3, "%f %f %f", &n.x, &n.y, &n.z) == 3)
                PUSH(m.norms, m.nnorms, m.capnorms, n);
        } else if (line[0] == 'f' && line[1] == ' ') {
            int vs[64], us[64], ns[64], cnt = 0;
            char* tok = strtok(line + 2, " \t\r\n");
            while (tok && cnt < 64) {
                parse_face_vertex(tok, &vs[cnt], &us[cnt], &ns[cnt]);
                cnt++;
                tok = strtok(NULL, " \t\r\n");
            }
            // Fan-triangulate (a triangular face emits exactly one triangle).
            for (int k = 1; k + 1 < cnt; ++k) {
                push_face(&m,
                          (Face){{vs[0], vs[k], vs[k + 1]}},
                          (Face){{us[0], us[k], us[k + 1]}},
                          (Face){{ns[0], ns[k], ns[k + 1]}});
            }
        }
    }
    fclose(f);
    *out = m;
    return 0;
}

void model_free(Model* m) {
    if (!m) return;
    free(m->verts);
    free(m->norms);
    free(m->uvs);
    free(m->faceV);
    free(m->faceUV);
    free(m->faceN);
    if (m->has_diffuse)   img_free(&m->diffuse);
    if (m->has_normalmap) img_free(&m->normalmap);
    memset(m, 0, sizeof *m);
}

int   model_nfaces(const Model* m) { return m->nfaces; }

Vec3f model_vert(const Model* m, int face, int nth) {
    return m->verts[m->faceV[face].idx[nth]];
}

Vec2f model_uv(const Model* m, int face, int nth) {
    if (m->nuvs == 0) return (Vec2f){0, 0};  // minimal .obj (no vt lines)
    return m->uvs[m->faceUV[face].idx[nth]];
}

Vec3f model_normal(const Model* m, int face, int nth) {
    if (m->nnorms == 0) return (Vec3f){0, 0, 1};  // no vn lines
    return m->norms[m->faceN[face].idx[nth]];
}

void model_set_diffuse(Model* m, const Image* tex) {
    if (m->has_diffuse) img_free(&m->diffuse);
    m->diffuse = img_copy(tex);
    m->has_diffuse = 1;
}

Color model_diffuse(const Model* m, Vec2f uv) {
    if (m->has_diffuse) {
        int x = (int)(uv.x * (float)(m->diffuse.w - 1));
        int y = (int)((1.0f - uv.y) * (float)(m->diffuse.h - 1));
        return img_get(&m->diffuse, x, y);
    }
    int c = ((int)(uv.x * 8) + (int)(uv.y * 8)) & 1;
    return c ? (Color){200, 200, 200} : (Color){60, 60, 60};
}

void model_set_normalmap(Model* m, const Image* nm) {
    if (m->has_normalmap) img_free(&m->normalmap);
    m->normalmap = img_copy(nm);
    m->has_normalmap = 1;
}

Vec3f model_normalmap(const Model* m, Vec2f uv) {
    if (!m->has_normalmap) return (Vec3f){0, 0, 1};
    int x = (int)(uv.x * (float)(m->normalmap.w - 1));
    int y = (int)((1.0f - uv.y) * (float)(m->normalmap.h - 1));
    Color c = img_get(&m->normalmap, x, y);
    return (Vec3f){
        (float)c.r / 255.0f * 2.0f - 1.0f,
        (float)c.g / 255.0f * 2.0f - 1.0f,
        (float)c.b / 255.0f * 2.0f - 1.0f,
    };
}

// ===================== gl.c =====================

Mat4 gl_viewport(int x, int y, int w, int h) {
    Mat4 r = mat4_identity();
    r.m[0][0] = (float)w / 2.0f;
    r.m[0][3] = (float)x + (float)w / 2.0f;
    // Negated (vs. textbook +h/2) to fold in the Y-flip (see 53's gl.hpp note).
    r.m[1][1] = -(float)h / 2.0f;
    r.m[1][3] = (float)y + (float)h / 2.0f;
    r.m[2][2] = 255.0f / 2.0f;
    r.m[2][3] = 255.0f / 2.0f;
    return r;
}

Mat4 gl_projection(float coeff) {
    Mat4 r = mat4_identity();
    r.m[3][2] = coeff;
    return r;
}

Mat4 gl_lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = vec3_normalize(vec3_sub(eye, center));
    Vec3f x = vec3_normalize(vec3_cross(up, z));
    Vec3f y = vec3_cross(z, x);

    Mat4 minv = mat4_identity();
    Mat4 tr = mat4_identity();
    minv.m[0][0] = x.x; minv.m[0][1] = x.y; minv.m[0][2] = x.z;
    minv.m[1][0] = y.x; minv.m[1][1] = y.y; minv.m[1][2] = y.z;
    minv.m[2][0] = z.x; minv.m[2][1] = z.y; minv.m[2][2] = z.z;
    tr.m[0][3] = -center.x; tr.m[1][3] = -center.y; tr.m[2][3] = -center.z;
    return mat4_mul(minv, tr);
}

void gl_line(int x0, int y0, int x1, int y1, Image* img, Color c) {
    int steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) { int t = x0; x0 = y0; y0 = t; t = x1; x1 = y1; y1 = t; }
    if (x0 > x1) { int t = x0; x0 = x1; x1 = t; t = y0; y0 = y1; y1 = t; }

    int dx = x1 - x0;
    int dy = abs(y1 - y0);
    int ystep = (y0 < y1) ? 1 : -1;
    int error = dx / 2;
    int y = y0;
    for (int x = x0; x <= x1; ++x) {
        if (steep) img_set(img, y, x, c);
        else       img_set(img, x, y, c);
        error -= dy;
        if (error < 0) { y += ystep; error += dx; }
    }
}

Vec3f gl_barycentric(Vec2f a, Vec2f b, Vec2f c, Vec2f p) {
    Vec3f u = vec3_cross((Vec3f){c.x - a.x, b.x - a.x, a.x - p.x},
                         (Vec3f){c.y - a.y, b.y - a.y, a.y - p.y});
    if (fabsf(u.z) < 1.0f) return (Vec3f){-1, 1, 1};
    return (Vec3f){1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z};
}

static int clampi(int v, int lo, int hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

void gl_triangle(const Vec4f clip[3], Shader* sh, Image* img, float* zbuf) {
    // Perspective divide (clip -> NDC) then viewport (NDC -> screen, depth 0..255).
    Mat4 vp = gl_viewport(0, 0, img->w, img->h);
    Vec3f screen[3];
    for (int i = 0; i < 3; ++i)
        screen[i] = vec4_proj3(mat4_mulv(vp, vec4_embed(vec4_proj3(clip[i]), 1.0f)));

    Vec2f a = {screen[0].x, screen[0].y};
    Vec2f b = {screen[1].x, screen[1].y};
    Vec2f cc = {screen[2].x, screen[2].y};

    float minXf = fminf(a.x, fminf(b.x, cc.x));
    float maxXf = fmaxf(a.x, fmaxf(b.x, cc.x));
    float minYf = fminf(a.y, fminf(b.y, cc.y));
    float maxYf = fmaxf(a.y, fmaxf(b.y, cc.y));

    int x0 = clampi((int)floorf(minXf), 0, img->w - 1);
    int x1 = clampi((int)ceilf(maxXf), 0, img->w - 1);
    int y0 = clampi((int)floorf(minYf), 0, img->h - 1);
    int y1 = clampi((int)ceilf(maxYf), 0, img->h - 1);

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            Vec3f w = gl_barycentric(a, b, cc, (Vec2f){(float)x, (float)y});
            if (w.x >= 0 && w.y >= 0 && w.z >= 0) {
                float z = w.x * screen[0].z + w.y * screen[1].z + w.z * screen[2].z;
                size_t idx = (size_t)x + (size_t)y * (size_t)img->w;
                if (z > zbuf[idx]) {
                    Color color;
                    if (!sh->fragment(sh, w, &color)) {
                        zbuf[idx] = z;
                        img_set(img, x, y, color);
                    }
                }
            }
        }
    }
}

// ===================== shader.c =====================

static float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

static Color shade(Color base, float intensity) {
    return (Color){
        (unsigned char)((float)base.r * intensity),
        (unsigned char)((float)base.g * intensity),
        (unsigned char)((float)base.b * intensity),
    };
}

static Vec2f interp_uv(const Vec2f uv[3], Vec3f bary) {
    return (Vec2f){
        bary.x * uv[0].x + bary.y * uv[1].x + bary.z * uv[2].x,
        bary.x * uv[0].y + bary.y * uv[1].y + bary.z * uv[2].y,
    };
}

// Lambert intensity of a (not-necessarily-unit) normal against a light dir.
static float lambert(Vec3f n, Vec3f light) {
    return clampf(vec3_dot(vec3_normalize(n), light), 0.0f, 1.0f);
}

static Vec3f interp_normal(const Vec3f n[3], Vec3f bary) {
    return vec3_add(vec3_add(vec3_scale(n[0], bary.x), vec3_scale(n[1], bary.y)),
                    vec3_scale(n[2], bary.z));
}

// --- FlatShader ---

static Vec4f flat_vertex(Shader* sh, int face, int nth) {
    FlatState* s = (FlatState*)sh->state;
    if (nth == 0) s->normalSum = (Vec3f){0, 0, 0};
    s->normalSum = vec3_add(s->normalSum, model_normal(s->model, face, nth));
    s->varyingUV[nth] = model_uv(s->model, face, nth);
    if (nth == 2) s->intensity = lambert(s->normalSum, s->light);
    return mat4_mulv(s->mvp, vec4_embed(model_vert(s->model, face, nth), 1.0f));
}

static int flat_fragment(Shader* sh, Vec3f bary, Color* out) {
    FlatState* s = (FlatState*)sh->state;
    *out = shade(model_diffuse(s->model, interp_uv(s->varyingUV, bary)), s->intensity);
    return 0;
}

Shader flat_shader(FlatState* s, const Model* model, Mat4 mvp, Vec3f light) {
    s->model = model; s->mvp = mvp; s->light = light;
    s->normalSum = (Vec3f){0, 0, 0}; s->intensity = 0.0f;
    Shader sh = {s, flat_vertex, flat_fragment};
    return sh;
}

// --- GouraudShader ---

static Vec4f gouraud_vertex(Shader* sh, int face, int nth) {
    GouraudState* s = (GouraudState*)sh->state;
    s->varyingUV[nth] = model_uv(s->model, face, nth);
    s->varyingIntensity[nth] = lambert(model_normal(s->model, face, nth), s->light);
    return mat4_mulv(s->mvp, vec4_embed(model_vert(s->model, face, nth), 1.0f));
}

static int gouraud_fragment(Shader* sh, Vec3f bary, Color* out) {
    GouraudState* s = (GouraudState*)sh->state;
    float intensity = bary.x * s->varyingIntensity[0] +
                      bary.y * s->varyingIntensity[1] +
                      bary.z * s->varyingIntensity[2];
    *out = shade(model_diffuse(s->model, interp_uv(s->varyingUV, bary)), intensity);
    return 0;
}

Shader gouraud_shader(GouraudState* s, const Model* model, Mat4 mvp, Vec3f light) {
    s->model = model; s->mvp = mvp; s->light = light;
    Shader sh = {s, gouraud_vertex, gouraud_fragment};
    return sh;
}

// --- PhongShader ---

static Vec4f phong_vertex(Shader* sh, int face, int nth) {
    PhongState* s = (PhongState*)sh->state;
    s->varyingUV[nth] = model_uv(s->model, face, nth);
    s->varyingNormal[nth] = model_normal(s->model, face, nth);
    return mat4_mulv(s->mvp, vec4_embed(model_vert(s->model, face, nth), 1.0f));
}

static int phong_fragment(Shader* sh, Vec3f bary, Color* out) {
    PhongState* s = (PhongState*)sh->state;
    Vec3f n = interp_normal(s->varyingNormal, bary);
    *out = shade(model_diffuse(s->model, interp_uv(s->varyingUV, bary)), lambert(n, s->light));
    return 0;
}

Shader phong_shader(PhongState* s, const Model* model, Mat4 mvp, Vec3f light) {
    s->model = model; s->mvp = mvp; s->light = light;
    Shader sh = {s, phong_vertex, phong_fragment};
    return sh;
}

// --- NormalMapShader ---

static Vec4f normalmap_vertex(Shader* sh, int face, int nth) {
    NormalMapState* s = (NormalMapState*)sh->state;
    s->varyingUV[nth] = model_uv(s->model, face, nth);
    s->varyingNormal[nth] = model_normal(s->model, face, nth);
    s->varyingPos[nth] = model_vert(s->model, face, nth);  // model==world (1.8 invariant)
    return mat4_mulv(s->mvp, vec4_embed(model_vert(s->model, face, nth), 1.0f));
}

static int normalmap_fragment(Shader* sh, Vec3f bary, Color* out) {
    NormalMapState* s = (NormalMapState*)sh->state;
    Vec3f N = vec3_normalize(interp_normal(s->varyingNormal, bary));

    // Triangle edges (world space) + matching UV gradients; solve for the
    // tangent T and bitangent B (same 2x2 UV-matrix inverse as 53's C++ shader).
    Vec3f e1 = vec3_sub(s->varyingPos[1], s->varyingPos[0]);
    Vec3f e2 = vec3_sub(s->varyingPos[2], s->varyingPos[0]);
    float du1 = s->varyingUV[1].x - s->varyingUV[0].x;
    float dv1 = s->varyingUV[1].y - s->varyingUV[0].y;
    float du2 = s->varyingUV[2].x - s->varyingUV[0].x;
    float dv2 = s->varyingUV[2].y - s->varyingUV[0].y;
    float det = du1 * dv2 - du2 * dv1;

    Vec3f n_t = model_normalmap(s->model, interp_uv(s->varyingUV, bary));
    Vec3f n_world;
    if (fabsf(det) < 1e-8f) {
        n_world = N;  // degenerate UVs: no tangent frame, fall back to smooth N
    } else {
        float inv = 1.0f / det;
        Vec3f T = vec3_scale(vec3_sub(vec3_scale(e1, dv2), vec3_scale(e2, dv1)), inv);
        // Gram-Schmidt orthonormalize T against N, then B = N x T.
        T = vec3_normalize(vec3_sub(T, vec3_scale(N, vec3_dot(T, N))));
        Vec3f B = vec3_cross(N, T);
        n_world = vec3_normalize(vec3_add(vec3_add(vec3_scale(T, n_t.x), vec3_scale(B, n_t.y)),
                                          vec3_scale(N, n_t.z)));
    }

    float intensity = clampf(vec3_dot(n_world, s->light), 0.0f, 1.0f);
    *out = shade(model_diffuse(s->model, interp_uv(s->varyingUV, bary)), intensity);
    return 0;
}

Shader normalmap_shader(NormalMapState* s, const Model* model, Mat4 mvp, Vec3f light) {
    s->model = model; s->mvp = mvp; s->light = light;
    Shader sh = {s, normalmap_vertex, normalmap_fragment};
    return sh;
}

// ===================== main.c =====================

// Numerically-checkable invariants, asserted at startup (Debug build). Mirrors
// 53's selfCheck(). Every Model/Image created here is freed so the run stays
// leak-clean under the sanitizer.
static void selfCheck(void) {
    Image t = img_create(4, 4);
    img_set(&t, 1, 2, (Color){10, 20, 30});
    Color c = img_get(&t, 1, 2);
    assert(c.r == 10 && c.g == 20 && c.b == 30);
    assert(img_get(&t, -5, -5).r == 0);  // clamp returns the (black) edge pixel
    img_free(&t);

    Vec3f a = {1, 0, 0}, b = {0, 1, 0};
    assert(vec3_dot(a, b) == 0.0f);
    Vec3f cr = vec3_cross(a, b);
    assert(cr.x == 0 && cr.y == 0 && cr.z == 1);
    assert(fabsf(vec3_norm(vec3_normalize((Vec3f){3, 4, 0})) - 1.0f) < 1e-6f);
    Mat4 I = mat4_identity();
    Vec4f v = mat4_mulv(I, (Vec4f){2, 3, 4, 1});
    assert(v.x == 2 && v.y == 3 && v.z == 4 && v.w == 1);

    Image line = img_create(10, 10);
    gl_line(0, 0, 9, 0, &line, (Color){255, 255, 255});
    assert(img_get(&line, 9, 0).r == 255);
    img_free(&line);

    Vec2f A = {0, 0}, B = {4, 0}, C = {0, 4};
    Vec3f w = gl_barycentric(A, B, C, (Vec2f){1, 1});
    assert(w.x > 0 && w.y > 0 && w.z > 0);
    assert(fabsf(w.x + w.y + w.z - 1.0f) < 1e-5f);
    Vec3f wo = gl_barycentric(A, B, C, (Vec2f){5, 5});
    assert(wo.x < 0 || wo.y < 0 || wo.z < 0);

    Model cube = model_cube();
    assert(model_nfaces(&cube) == 12);
    model_free(&cube);

    Model bare = model_cube();
    Color chk = model_diffuse(&bare, (Vec2f){0.1f, 0.1f});
    assert((chk.r == 200 && chk.g == 200 && chk.b == 200) ||
           (chk.r == 60 && chk.g == 60 && chk.b == 60));
    Image white = img_create(2, 2);
    for (int i = 0; i < 4; ++i) white.px[i] = (Color){255, 255, 255};
    model_set_diffuse(&bare, &white);
    Color s = model_diffuse(&bare, (Vec2f){0.5f, 0.5f});
    assert(s.r == 255 && s.g == 255 && s.b == 255);
    img_free(&white);
    model_free(&bare);

    Mat4 p0 = gl_projection(0.0f), id = mat4_identity();
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) assert(fabsf(p0.m[i][j] - id.m[i][j]) < 1e-6f);

    // viewport(0,0,400,400): NDC (-1,-1,-1)->(0,400,0), (1,1,1)->(400,0,255).
    // (Depth 0 = farthest, 255 = nearest — this renderer is larger-z-nearer.)
    Mat4 vpm = gl_viewport(0, 0, 400, 400);
    Vec3f lo = vec4_proj3(mat4_mulv(vpm, vec4_embed((Vec3f){-1, -1, -1}, 1.0f)));
    Vec3f hi = vec4_proj3(mat4_mulv(vpm, vec4_embed((Vec3f){1, 1, 1}, 1.0f)));
    assert(fabsf(lo.x - 0.0f) < 1e-4f && fabsf(lo.y - 400.0f) < 1e-4f && fabsf(lo.z - 0.0f) < 1e-4f);
    assert(fabsf(hi.x - 400.0f) < 1e-4f && fabsf(hi.y - 0.0f) < 1e-4f && fabsf(hi.z - 255.0f) < 1e-4f);

    Mat4 view = gl_lookat((Vec3f){1, 1, 3}, (Vec3f){0, 0, 0}, (Vec3f){0, 1, 0});
    Vec3f r0 = {view.m[0][0], view.m[0][1], view.m[0][2]};
    Vec3f r1 = {view.m[1][0], view.m[1][1], view.m[1][2]};
    Vec3f r2 = {view.m[2][0], view.m[2][1], view.m[2][2]};
    assert(fabsf(vec3_norm(r0) - 1.0f) < 1e-5f && fabsf(vec3_norm(r1) - 1.0f) < 1e-5f &&
           fabsf(vec3_norm(r2) - 1.0f) < 1e-5f);
    assert(fabsf(vec3_dot(r0, r1)) < 1e-5f && fabsf(vec3_dot(r1, r2)) < 1e-5f &&
           fabsf(vec3_dot(r0, r2)) < 1e-5f);

    Model bareNM = model_cube();
    Vec3f n = model_normalmap(&bareNM, (Vec2f){0.3f, 0.7f});
    assert(n.x == 0.0f && n.y == 0.0f && n.z == 1.0f);
    Image flat = img_create(2, 2);
    for (int i = 0; i < 4; ++i) flat.px[i] = (Color){128, 128, 255};
    model_set_normalmap(&bareNM, &flat);
    Vec3f nn = model_normalmap(&bareNM, (Vec2f){0.5f, 0.5f});
    float tol = 1.0f / 255.0f * 2.0f;
    assert(fabsf(nn.x) < tol && fabsf(nn.y) < tol && fabsf(nn.z - 1.0f) < tol);
    img_free(&flat);
    model_free(&bareNM);
}

int main(int argc, char** argv) {
    selfCheck();
    const int W = 400, H = 400;
    Image img = img_create(W, H);
    float* zbuf = (float*)malloc((size_t)W * (size_t)H * sizeof(float));
    for (size_t i = 0; i < (size_t)W * (size_t)H; ++i) zbuf[i] = -INFINITY;

    // --- Model + optional textures from the command line ---
    // (full arg list in assets/tinyrenderer/README.md)
    //   argv[1]  model:  "cube" (default) | "sphere" | path to a .obj
    //   argv[2]  shader: "flat" | "gouraud" | "phong" | "normal" (default)
    //   argv[3]  P6 PPM diffuse texture   ("-" keeps the procedural checkerboard)
    //   argv[4]  P6 PPM tangent-space normal map (else a flat, no-op map)
    const char* modelArg = (argc > 1) ? argv[1] : "cube";
    Model model;
    if (strcmp(modelArg, "cube") == 0) {
        model = model_cube();
    } else if (strcmp(modelArg, "sphere") == 0) {
        model = model_sphere(24, 24);
    } else if (model_load_obj(&model, modelArg) != 0) {
        fprintf(stderr, "65-renderer-flat: cannot load model: %s\n", modelArg);
        img_free(&img);
        free(zbuf);
        return 1;
    }

    if (argc > 3 && strcmp(argv[3], "-") != 0) {
        Image tex;
        if (img_read_ppm(&tex, argv[3]) != 0) {
            fprintf(stderr, "65-renderer-flat: cannot load diffuse: %s\n", argv[3]);
            model_free(&model); img_free(&img); free(zbuf);
            return 1;
        }
        model_set_diffuse(&model, &tex);
        img_free(&tex);
    }
    if (argc > 4 && strcmp(argv[4], "-") != 0) {
        Image nm;
        if (img_read_ppm(&nm, argv[4]) != 0) {
            fprintf(stderr, "65-renderer-flat: cannot load normal map: %s\n", argv[4]);
            model_free(&model); img_free(&img); free(zbuf);
            return 1;
        }
        model_set_normalmap(&model, &nm);
        img_free(&nm);
    }
    const char* shaderName = (argc > 2) ? argv[2] : "normal";

    // Camera: 3/4 view with perspective foreshortening. coeff = -1/eye.z.
    Vec3f eye = {1.0f, 1.0f, 3.0f};
    Vec3f center = {0.0f, 0.0f, 0.0f};
    Vec3f up = {0.0f, 1.0f, 0.0f};
    float coeff = -1.0f / eye.z;

    // Autofit any model: scale its largest |coordinate| to ~0.6 so it lands
    // inside the [-1,1] NDC box (a unit ±1 cube -> 0.6, as in 53). Uniform scale,
    // so normals are untouched.
    float maxExtent = 1e-6f;
    for (int f = 0; f < model_nfaces(&model); ++f)
        for (int k = 0; k < 3; ++k) {
            Vec3f vv = model_vert(&model, f, k);
            maxExtent = fmaxf(maxExtent, fmaxf(fabsf(vv.x), fmaxf(fabsf(vv.y), fabsf(vv.z))));
        }
    float modelScale = 0.6f / maxExtent;

    Mat4 view = gl_lookat(eye, center, up);
    Mat4 proj = gl_projection(coeff);
    Mat4 scale = mat4_identity();
    scale.m[0][0] = scale.m[1][1] = scale.m[2][2] = modelScale;
    Mat4 mvp = mat4_mul(mat4_mul(proj, view), scale);

    Vec3f light = vec3_normalize((Vec3f){0.3f, 0.5f, 1.0f});

    // One shader's state is live per run; a union avoids declaring the unused
    // three (which would trip -Wunused-variable).
    union {
        FlatState f;
        GouraudState g;
        PhongState p;
        NormalMapState n;
    } st;
    Shader sh;
    if (strcmp(shaderName, "flat") == 0) {
        sh = flat_shader(&st.f, &model, mvp, light);
    } else if (strcmp(shaderName, "gouraud") == 0) {
        sh = gouraud_shader(&st.g, &model, mvp, light);
    } else if (strcmp(shaderName, "phong") == 0) {
        sh = phong_shader(&st.p, &model, mvp, light);
    } else {
        sh = normalmap_shader(&st.n, &model, mvp, light);  // default
    }

    for (int f = 0; f < model_nfaces(&model); ++f) {
        Vec4f clip[3];
        for (int k = 0; k < 3; ++k) clip[k] = sh.vertex(&sh, f, k);
        gl_triangle(clip, &sh, &img, zbuf);
    }

    img_write_ppm(&img, "render.ppm");

    model_free(&model);
    img_free(&img);
    free(zbuf);
    return 0;
}
