# mandelbrot

Draws the Mandelbrot set as ASCII art: for every character cell, iterate
`z = z² + c` and shade by how long the point takes to escape. It's the same
"math → picture, one pixel at a time" idea as the renderer capstone, boiled down
to ~30 lines and the terminal.

**Builds on:** operators/math (06), for-loop (12), strings (20); a gentle lead-in
to the renderer (63) and its PPM output.

## Run
```sh
make run-cpp-app-mandelbrot            # 78 x 28
make run-cpp-app-mandelbrot ARGS="120 40"
```

## Try extending it
- Zoom: make the plane bounds command-line arguments and zoom into a seahorse.
- Write a PPM image instead of ASCII (reuse the renderer's `image` idea, lesson 63).
- Colour by escape count using ANSI escape codes.
