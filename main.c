#include "sdl-interface.h"
#include "stack.h"
#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Renderer *renderer;

void swapPoint2d(Point2d *a, Point2d *b) {
  int temp;
  temp = a->x;
  a->x = b->x;
  b->x = temp;
  temp = a->y;
  a->y = b->y;
  b->y = temp;
}

Stack *interpolate(int i0, int d0, int i1, int d1) {
  // i is the independant variable, d is the value we want to compute
  if (i0 == i1) {
    Stack *values = createStack(1);
    push(values, d0);
    return values;
  }

  Stack *values = createStack(abs(i1 - i0) + 1);
  float slope = (float)(d1 - d0) / (float)(i1 - i0);
  float d = d0;

  for (int i = i0; i <= i1; i++) {
    push(values, (int)d);
    d = d + slope;
  }

  return values;
}

void drawLine(Point2d p0, Point2d p1, const Color *color) {
  // we need to version of the algorithm: y = f(x) and x = f(y)
  int dx = p1.x - p0.x;
  int dy = p1.y - p1.x;

  if (abs(dx) > abs(dy)) {
    if (p0.x > p1.x) {
      int temp = p0.x;
      p0.x = p1.x;
      p1.x = temp;
    }

    Stack *ys = interpolate(p0.x, p0.y, p1.x, p1.y);
    for (int x = p0.x; x <= p1.x; ++x)
      setPixel(renderer, (Point2d){x, ys->array[x - p0.x]}, color);

  } else {
    if (p0.y > p1.y) {
      int temp = p0.y;
      p0.y = p1.y;
      p1.y = temp;
    }

    Stack *xs = interpolate(p0.y, p0.x, p1.y, p1.x);
    for (int y = p0.y; y <= p1.y; ++y)
      setPixel(renderer, (Point2d){xs->array[y - p0.y], y}, color);
  }
}

void drawWireframeTriangle(Point2d p0, Point2d p1, Point2d p2,
                           const Color *color) {
  drawLine(p0, p1, color);
  drawLine(p1, p2, color);
  drawLine(p2, p0, color);
}

void drawTriangle(Point2d p0, Point2d p1, Point2d p2, const Color *color) {
  // draw each vertical line inside the triangle p0.y + (x>0) = p2.y
  if (p1.y < p0.y)
    swapPoint2d(&p0, &p1);
  if (p2.y < p0.y)
    swapPoint2d(&p0, &p2);
  if (p2.y < p1.y)
    swapPoint2d(&p1, &p2);

  Stack *x012 = interpolate(p0.y, p0.x, p1.y, p1.x);
  pop(x012); // the last value is the same than the first value of x12
  Stack *x12 = interpolate(p1.y, p1.x, p2.y, p2.x);
  append(x012, x12, x12->capacity);
  Stack *x02 = interpolate(p0.y, p0.x, p2.y, p2.x);
  // the value from one of the side comes from 02, for the other side, from the
  // concatenation of 01 and 12

  Stack *xLeft = NULL;
  Stack *xRight = NULL;

  // we need to determine whether x02 or x012 is the right/left side
  int m = floor((float)x02->top / 2);
  if (x02->array[m] < x012->array[m]) {
    xLeft = x02;
    xRight = x012;
  } else {
    xLeft = x012;
    xRight = x02;
  }

  if (xLeft == NULL || xRight == NULL) {
    printf("Failed to draw triangle.");
    exit(EXIT_FAILURE);
  }

  for (int y = p0.y; y <= p2.y; ++y) {
    for (int x = xLeft->array[y - p0.y]; x <= xRight->array[y - p0.y]; ++x) {
      setPixel(renderer, (Point2d){x, y}, color);
    }
  }

  destroyStack(x012);
  destroyStack(x02);
}

int main(void) {
  printf("Render width: %d\n", RENDER_WIDTH);

  // initialize SDL

  SDL_Event event;
  SDL_Window *window;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window,
                              &renderer);
  clearCanva(renderer);
  SDL_SetWindowTitle(window, "window");

  // render the scene

  Color color = {25, 55, 255};
  drawLine((Point2d){-200, -100}, (Point2d){240, 120}, &color);
  drawLine((Point2d){0, -200}, (Point2d){0, 200}, &color);
  drawLine((Point2d){-50, -200}, (Point2d){60, 240}, &color);

  drawTriangle((Point2d){0, 0}, (Point2d){120, 120}, (Point2d){120, 0}, &color);

  SDL_RenderPresent(renderer);

  while (1) {

    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      break;
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
