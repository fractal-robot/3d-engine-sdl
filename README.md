
![rendering-engine](https://github.com/fractal-robot/3d-engine-sdl/assets/146975654/b05abbbd-a673-4ff9-aaa0-aa93a1aa29b9)

This is my attempt at making a rasterizer from scratch using C and SDL.
I'm only allowed to use SDL for capturing controls, and as an interface that allow me to set a specific pixel on the window.

This rendering engine features for now:
- Drawing pixels, lines; wireframe, full, and shade triangles.
- Self-written stack and matrix data structure.
- Drawing 3D wireframe or not (a depth buffer is implemented) instances onto the screen.
- Model/Instance structure: each instance is a model with some transformations (scale, rotation and translation matrix for now).
- The camera can be (only) *translated* using WSQD and E + space for the height.
- 1 step 5 planes clipping (only not rendering the instances whose bounding spheres are outside the clipping space).
- Optimized calculation of the instance data, the model transformation matrix gets computed only if a new transformation has been set, and the bounding sphere only if the shape is scaled. 
 
# Compilation instructions:

```
cd build && make
../bin/debug/rasterizer
```
