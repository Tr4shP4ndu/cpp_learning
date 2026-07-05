#ifndef IMAGE_H
#define IMAGE_H

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

#endif // IMAGE_H
