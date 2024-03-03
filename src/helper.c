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
#include <stdlib.h>

// we conpute the eucledian distance from the center of the model, so p2 is
// constantly equal to 0. This computation should be performed after the scaling
// only.
float distanceToCenter(float3d center, const Mat *p) {
  assert(p->rows == 4 && p->cols == 1);

  return sqrt(pow(getX(p) - center.x, 2) + pow(getY(p) - center.y, 2) +
              pow(getZ(p) - center.z, 2));
}

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

void drawLine(Mat *p0, Mat *p1, const Color *color) {
  assert(p0->rows == 2 && p1->rows == 2 && p0->cols == 1 && p1->cols == 1);

  int dx = getX(p1) - getX(p0);
  int dy = getY(p1) - getY(p0);

  Mat *pixelCoord = createMat(2, 1, false);

  if (abs(dx) > abs(dy)) {
    if (getX(p0) > getX(p1))
      swapMatPointers(&p0, &p1);

    Stack *ys = interpolate(getX(p0), getY(p0), getX(p1), getY(p1));
    for (int x = getX(p0); x <= getX(p1); ++x) {
      int y = *(int *)getStackItem(ys, x - p0->data[0][0]);
      setX(pixelCoord, x);
      setY(pixelCoord, y);
      setPixel(pixelCoord, color);
    }

    for (int i = 0; i <= ys->top; ++i)
      free(ys->items[i]);
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
      setPixel(pixelCoord, color);
    }
    for (int i = 0; i <= xs->top; ++i)
      free(xs->items[i]);

    freeStack(xs);
  }

  freeMat(pixelCoord);
}

void drawWireframeTriangle(Mat *p0, Mat *p1, Mat *p2, const Color *color) {
  drawLine(p0, p1, color);
  drawLine(p1, p2, color);
  drawLine(p2, p0, color);
}

void drawTriangle(Mat *p0, Mat *p1, Mat *p2, const Color *color) {
  assert(p0->rows == 2 && p1->rows == 2 && p2->rows == 2 && p0->cols == 1 &&
         p1->cols == 1 && p2->cols == 1);

  // draw each vertical line inside the triangle p0->data[0][1] + (x>0) =
  // p2->data[0][1]
  if (p1->data[1][0] < p0->data[1][0])
    swapMatPointers(&p0, &p1);
  if (p2->data[1][0] < p0->data[1][0])
    swapMatPointers(&p0, &p2);
  if (p2->data[1][0] < p1->data[1][0])
    swapMatPointers(&p1, &p2);

  Stack *x012 = interpolate(p0->data[1][0], p0->data[0][0], p1->data[1][0],
                            p1->data[0][0]);
  pop(x012); // the last value is the same than the first value of x12
  Stack *x12 = interpolate(p1->data[1][0], p1->data[0][0], p2->data[1][0],
                           p2->data[0][0]);
  append(x012, x12);

  Stack *x02 = interpolate(p0->data[1][0], p0->data[0][0], p2->data[1][0],
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

  Mat *pixelCoord = createMat(2, 1, false);

  for (int y = p0->data[1][0]; y < p2->data[1][0]; ++y) {
    int fromX = *(int *)getStackItem(xLeft, y - p0->data[1][0]);
    int toX = *(int *)getStackItem(xRight, y - p0->data[1][0]);
    for (int x = fromX; x <= toX; ++x) {
      pixelCoord->data[0][0] = x;
      pixelCoord->data[1][0] = y;
      setPixel(pixelCoord, color);
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

void drawShadeTriangle(Mat *p0, Mat *p1, Mat *p2, float h0, float h1, float h2,
                       const Color *color) {
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
      setPixel(pixelCoord, &shadedColor);
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
  setX(toCanva, getX(point) * (int)RENDER_WIDTH / VIEWPORT_WIDTH);
  setY(toCanva, getY(point) * (int)RENDER_WIDTH / VIEWPORT_WIDTH);
  return toCanva;
}

Mat *projectVertex(const Mat *point) {
  assert(point->rows == 4 && point->cols == 1);
  Mat *projected = createMat(2, 1, false);
  setX(projected, getX(point) * D / getZ(point));
  setY(projected, getY(point) * D / getZ(point));
  Mat *toCava = viewPortToCanva(projected);
  freeMat(projected);
  return toCava;
}
