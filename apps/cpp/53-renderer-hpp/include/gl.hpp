#pragma once
#include "image.hpp"

// Draws a straight line from (x0,y0) to (x1,y1) into img using color c.
// Integer Bresenham: no floating point, handles all octants (steep slopes
// and either endpoint ordering).
void drawLine(int x0, int y0, int x1, int y1, Image& img, Color c);
