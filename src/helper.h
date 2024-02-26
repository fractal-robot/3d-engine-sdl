#ifndef HELPER_H
#define HELPER_H

#include "stack.h"
#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

void swapMat2d(Mat2d *a, Mat2d *b);
void swapMat2dCarryHue(Mat2dCarryHue *a, Mat2dCarryHue *b);
Color shadeColor(const Color *color, float s);
Stack *interpolateColor(int i0, float d0, int i1, float d1);
Stack *interpolate(int i0, int d0, int i1, int d1);
void drawLine(SDL_Renderer *renderer, Mat2d p0, Mat2d p1, const Color *color);
void drawWireframeTriangle(SDL_Renderer *renderer, Mat2d p0, Mat2d p1, Mat2d p2,
                           const Color *color);
void drawTriangle(SDL_Renderer *renderer, Mat2d p0, Mat2d p1, Mat2d p2,
                  const Color *color);
void drawShadeTriangle(SDL_Renderer *renderer, Mat2dCarryHue p0,
                       Mat2dCarryHue p1, Mat2dCarryHue p2, const Color *color);
Mat2d viewPortToCanva(Mat2d point);
Mat2d projectVertex(const Mat3d *point);
Mat3d translate(Mat3d point);

#endif /* ifndef HELPER_H */
