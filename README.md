This is my attempt at making a rasterizer from scratch using C and SDL.
I'm only allowed to use SDL for capturing controls, and as an interface that allow me to set a specific pixel on the window.

This rendering engine features for now:
- Drawing pixels, lines; wireframe, full, and shade triangles.
- Self-written stack and matrix data structure.
- Drawing 3D wireframe instances onto the screen.
- Model/Instance structure: each instance is a model with some transformations (scale, rotation and translation for now).
- The camera can be (only, for now) *translated* using WSQD and E + space for the height.
- 1 step clipping (only not rendering the instances whose bounding spheres are outside the clipping space (5 planes clipping space), not create new triangles based on clipping data).
- Optimized calculation of the instance data, the model transformation matrix gets computed only if a new transformation has been set, and the bounding sphere only if the shape is scaled. 
 
# Compilation instructions:

I still need to learn how to make my Makefile more portable, but you can compile with the following commands (example with arch):
```
sudo pacman -S sdl2
sudo git clone ... 
cd 3d-engine-sdl
gcc src/*.c src/*.h -o main -lSDL2 -lm -O3
./main
```
