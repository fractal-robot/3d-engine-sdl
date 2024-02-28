#include "helper.h"
#include "assert.h"
#include "definition.h"
#include "matrix.h"
#include "sdl-interface.h"
#include "stack.h"
#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

Color shadeColor(const Color *col, float shade) {
  return (Color){col->red * shade, col->green * shade, col->blue * shade};
}

Stack *interpolateColor(int i0, float d0, int i1, float d1) {
  // i is the independant variable, d is the value we want to compute
  if (i0 == i1) {
    Stack *values = createStack(1);
    pushLocal(values, &d0, sizeof(d0));
    return values;
  }

  Stack *values = createStack(abs(i1 - i0));
  float slope = (d1 - d0) / (float)(i1 - i0);
  float d = d0;

  for (int i = i0; i <= i1; i++) {
    pushLocal(values, &d, sizeof(d));
    d = d + slope;
  }

  return values;
}

Stack *interpolate(int i0, int d0, int i1, int d1) {
  // i is the independant variable, d is the value we want to compute
  if (i0 == i1) {
    Stack *values = createStack(1);
    pushLocal(values, &d0, sizeof(d0));
    return values;
  }

  Stack *values = createStack(abs(i1 - i0));
  float slope = (float)(d1 - d0) / (float)(i1 - i0);
  float d = d0;

  for (int i = i0; i <= i1; i++) {
    int equivalent = floor(d);
    pushLocal(values, &equivalent, sizeof(equivalent));
    d = d + slope;
  }

  return values;
}

void swapMatPointers(Mat **p0, Mat **p1) {
  Mat *temp = *p0;
  *p0 = *p1;
  *p1 = temp;
}

void drawLine(SDL_Renderer *renderer, Mat *p0, Mat *p1, const Color *color) {
  assert(p0->rows == 2 && p1->rows == 2 && p0->cols == 1 && p1->cols == 1);

  int dx = p1->data[0][0] - p0->data[0][0];
  int dy = p1->data[1][0] - p0->data[1][0];

  Mat *pixelCoord = createMat(2, 1, false);

  if (abs(dx) > abs(dy)) {
    if (p0->data[0][0] > p1->data[0][0])
      swapMatPointers(&p0, &p1);

    Stack *ys = interpolate(p0->data[0][0], p0->data[1][0], p1->data[0][0],
                            p1->data[1][0]);
    for (int x = p0->data[0][0]; x <= p1->data[0][0]; ++x) {
      int y = *(int *)getStackItem(ys, x - p0->data[0][0]);
      pixelCoord->data[0][0] = x;
      pixelCoord->data[1][0] = y;
      setPixel(renderer, pixelCoord, color);
    }
    freeStack(ys);

  } else {
    if (p0->data[1][0] > p1->data[1][0])
      swapMatPointers(&p0, &p1);

    Stack *xs = interpolate(p0->data[1][0], p0->data[0][0], p1->data[1][0],
                            p1->data[0][0]);
    for (int y = p0->data[1][0]; y <= p1->data[1][0]; ++y) {
      int x = *(int *)getStackItem(xs, y - p0->data[1][0]);
      pixelCoord->data[0][0] = x;
      pixelCoord->data[1][0] = y;
      setPixel(renderer, pixelCoord, color);
    }
    freeStack(xs);
  }

  freeMat(pixelCoord);
}

void drawWireframeTriangle(SDL_Renderer *renderer, Mat *p0, Mat *p1, Mat *p2,
                           const Color *color) {
  drawLine(renderer, p0, p1, color);
  drawLine(renderer, p1, p2, color);
  drawLine(renderer, p2, p0, color);
}

void drawTriangle(SDL_Renderer *renderer, Mat *p0, Mat *p1, Mat *p2,
                  const Color *color) {
  assert(p0->rows == 1 && p1->rows == 1 && p2->rows == 1 && p0->cols == 2 &&
         p1->cols == 2 && p2->cols == 2);

  // draw each vertical line inside the triangle p0->data[0][1] + (x>0) =
  // p2->data[0][1]
  if (p1->data[0][1] < p0->data[0][1])
    swapMatPointers(&p0, &p1);
  if (p2->data[0][1] < p0->data[0][1])
    swapMatPointers(&p0, &p2);
  if (p2->data[0][1] < p1->data[0][1])
    swapMatPointers(&p1, &p2);

  Stack *x012 = interpolate(p0->data[0][1], p0->data[0][0], p1->data[0][1],
                            p1->data[0][0]);
  pop(x012); // the last value is the same than the first value of x12
  Stack *x12 = interpolate(p1->data[0][1], p1->data[0][0], p2->data[0][1],
                           p2->data[0][0]);
  append(x012, x12);

  Stack *x02 = interpolate(p0->data[0][1], p0->data[0][0], p2->data[0][1],
                           p2->data[0][0]);
  // the value from one of the side comes from 02, for the other side, from the
  // concatenation of 01 and 12

  Stack *xLeft;
  Stack *xRight;

  // we need to determine whether x02 or x012 is the right/left side
  int m = floor((float)x02->top / 2);
  if (*(int *)getStackItem(x02, m) < *(int *)getStackItem(x012, m)) {
    xLeft = x02;
    xRight = x012;
  } else {
    xLeft = x012;
    xRight = x02;
  }

  Mat *pixelCoord = createMat(1, 2, false);

  for (int y = p0->data[0][1]; y < p2->data[0][1]; ++y) {
    int fromX = *(int *)getStackItem(xLeft, y - p0->data[0][1]);

    int toX = *(int *)getStackItem(xRight, y - p0->data[0][1]);
    for (int x = fromX; x <= toX; ++x) {
      pixelCoord->data[0][0] = x;
      pixelCoord->data[0][1] = y;
      setPixel(renderer, pixelCoord, color);
    }
  }

  freeStack(x012);
  freeStack(x02);
  freeMat(pixelCoord);
}

void swapFloats(float *a, float *b) {
  float temp = *a;
  *a = *b;
  *b = temp;
}

void drawShadeTriangle(SDL_Renderer *renderer, Mat *p0, Mat *p1, Mat *p2,
                       float h0, float h1, float h2, const Color *color) {
  // sort the vertices from bottom to top
  if (p1->data[0][1] < p0->data[0][1]) {
    swapMatPointers(&p0, &p1);
    swapFloats(&h0, &h1);
  }
  if (p2->data[0][1] < p0->data[0][1]) {
    swapMatPointers(&p0, &p2);
    swapFloats(&h0, &h2);
  }
  if (p2->data[0][1] < p1->data[0][1]) {
    swapMatPointers(&p1, &p2);
    swapFloats(&h1, &h2);
  }

  Stack *x012 = interpolate(p0->data[0][1], p0->data[0][0], p1->data[0][1],
                            p1->data[0][0]);
  pop(x012); // the last value is the same than the first value of x12
  Stack *x12 = interpolate(p1->data[0][1], p1->data[0][0], p2->data[0][1],
                           p2->data[0][0]);
  append(x012, x12);

  Stack *x02 = interpolate(p0->data[0][1], p0->data[0][0], p2->data[0][1],
                           p2->data[0][0]);

  Stack *h012 = interpolateColor(p0->data[0][1], h0, p1->data[0][1], h1);
  pop(h012); // the last value is the same than the first value of x12
  Stack *h12 = interpolateColor(p1->data[0][1], h1, p2->data[0][1], h2);
  append(h012, h12);
  Stack *h02 = interpolateColor(p0->data[0][1], h0, p2->data[0][1], h2);

  Stack *xLeft;
  Stack *hLeft;
  Stack *xRight;
  Stack *hRight;

  // we need to determine whether x02 or x012 is the right/left side
  int m = floor((float)x02->top / 2);
  if (*(int *)getStackItem(x02, m) < *(int *)getStackItem(x012, m)) {
    xLeft = x02;
    hLeft = h02;
    xRight = x012;
    hRight = h012;
  } else {
    xLeft = x012;
    hLeft = h012;
    xRight = x02;
    hRight = h02;
  }

  Mat *pixelCoord = createMat(1, 2, false);

  for (int y = p0->data[0][1]; y < p2->data[0][1]; ++y) {
    int fromX = *(int *)getStackItem(xLeft, y - p0->data[0][1]);
    int toX = *(int *)getStackItem(xRight, y - p0->data[0][1]);
    Stack *hueSegment = interpolateColor(
        fromX, *(float *)getStackItem(hLeft, y - p0->data[0][1]), toX,
        *(float *)getStackItem(hRight, y - p0->data[0][1]));
    for (int x = fromX; x <= toX; ++x) {
      Color shadedColor =
          shadeColor(color, *(float *)getStackItem(hueSegment, x - fromX));
      pixelCoord->data[0][0] = x;
      pixelCoord->data[0][1] = y;
      setPixel(renderer, pixelCoord, &shadedColor);
    }
  }

  freeStack(x012);
  freeStack(h012);
  freeStack(x02);
  freeStack(h02);
  freeMat(pixelCoord);
}

Mat *viewPortToCanva(const Mat *point) {
  Mat *toCanva = createMat(2, 1, false);
  toCanva->data[0][0] = point->data[0][0] * (int)RENDER_WIDTH / VIEWPORT_WIDTH;
  toCanva->data[1][0] = point->data[1][0] * (int)RENDER_WIDTH / VIEWPORT_WIDTH;
  return toCanva;
}

Mat *projectVertex(const Mat *point) {
  assert(point->rows == 4 && point->cols == 1);
  const Mat *projected = createMat(2, 1, false);
  projected->data[0][0] = point->data[0][0] * D / point->data[2][0];
  projected->data[1][0] = point->data[1][0] * D / point->data[2][0];
  Mat *toCava = viewPortToCanva(projected);
  freeMat(projected);
  return toCava;
}
