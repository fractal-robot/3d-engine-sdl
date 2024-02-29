#ifndef HELPER_H
#define HELPER_H

#include "matrix.h"
#include "stack.h"
#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

Color shadeColor(const Color *col, float shade);
Stack *interpolateColor(int i0, float d0, int i1, float d1);
Stack *interpolate(int i0, int d0, int i1, int d1);
void swapMatPointers(Mat **p0, Mat **p1);
void swapColPointers(Color **c0, Color **c1);
void drawLine(SDL_Renderer *renderer, Mat *p0, Mat *p1, const Color *color);
void drawWireframeTriangle(SDL_Renderer *renderer, Mat *p0, Mat *p1, Mat *p2,
                           const Color *color);
void drawTriangle(SDL_Renderer *renderer, Mat *p0, Mat *p1, Mat *p2,
                  const Color *color);
Mat *viewPortToCanva(const Mat *point);
Mat *projectVertex(const Mat *point);
void drawShadeTriangle(SDL_Renderer *renderer, Mat *p0, Mat *p1, Mat *p2,
                       float h0, float h1, float h2, const Color *color);
float findDistanceToCenter(const float3d *point);

#endif
