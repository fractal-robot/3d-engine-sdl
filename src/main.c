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

void swapPoint2dCarryHue(Point2dCarryHue *a, Point2dCarryHue *b) {
  int temp;
  temp = a->x;
  a->x = b->x;
  b->x = temp;
  temp = a->y;
  a->y = b->y;
  b->y = temp;
  temp = a->hue;
  a->hue = b->hue;
  b->hue = temp;
}

Color shadeColor(const Color *color, float s) {
  return (Color){color->red * s, color->green * s, color->blue * s};
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

void drawLine(Point2d p0, Point2d p1, const Color *color) {
  // we need to version of the algorithm: y = f(x) and x = f(y)
  int dx = p1.x - p0.x;
  int dy = p1.y - p0.y;

  if (abs(dx) > abs(dy)) {
    if (p0.x > p1.x)
      swapPoint2d(&p0, &p1);

    Stack *ys = interpolate(p0.x, p0.y, p1.x, p1.y);
    for (int x = p0.x; x <= p1.x; ++x) {
      int y = *(int *)getStackItem(ys, x - p0.x);
      setPixel(renderer, (Point2d){x, y}, color);
    }

  } else {
    if (p0.y > p1.y)
      swapPoint2d(&p0, &p1);

    Stack *xs = interpolate(p0.y, p0.x, p1.y, p1.x);
    for (int y = p0.y; y <= p1.y; ++y) {
      int x = *(int *)getStackItem(xs, y - p0.y);

      setPixel(renderer, (Point2d){x, y}, color);
    }
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
  printf("size of 01: %i\n", x012->top);
  pop(x012); // the last value is the same than the first value of x12
  Stack *x12 = interpolate(p1.y, p1.x, p2.y, p2.x);
  printf("size of 12: %i\n", x12->top);
  append(x012, x12);
  printf("size of 012: %i\n", x012->top);

  Stack *x02 = interpolate(p0.y, p0.x, p2.y, p2.x);
  printf("size of 02: %i\n", x02->top);
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

  for (int y = p0.y; y < p2.y; ++y) {
    int fromX = *(int *)getStackItem(xLeft, y - p0.y);

    int toX = *(int *)getStackItem(xRight, y - p0.y);
    for (int x = fromX; x <= toX; ++x)
      setPixel(renderer, (Point2d){x, y}, color);
  }

  deleteStack(x012);
  deleteStack(x02);
}

void drawShadeTriangle(Point2dCarryHue p0, Point2dCarryHue p1,
                       Point2dCarryHue p2, const Color *color) {
  // draw each vertical line inside the triangle p0.y + (x>0) = p2.y
  if (p1.y < p0.y)
    swapPoint2dCarryHue(&p0, &p1);
  if (p2.y < p0.y)
    swapPoint2dCarryHue(&p0, &p2);
  if (p2.y < p1.y)
    swapPoint2dCarryHue(&p1, &p2);

  Stack *x012 = interpolate(p0.y, p0.x, p1.y, p1.x);
  printf("size of 01: %i\n", x012->top);
  pop(x012); // the last value is the same than the first value of x12
  Stack *x12 = interpolate(p1.y, p1.x, p2.y, p2.x);
  printf("size of 12: %i\n", x12->top);
  append(x012, x12);
  printf("size of 012: %i\n", x012->top);

  Stack *x02 = interpolate(p0.y, p0.x, p2.y, p2.x);
  printf("size of 02: %i\n", x02->top);

  Stack *h012 = interpolateColor(p0.y, p0.hue, p1.y, p1.hue);
  pop(h012); // the last value is the same than the first value of x12
  Stack *h12 = interpolateColor(p1.y, p1.hue, p2.y, p2.hue);
  append(h012, h12);
  Stack *h02 = interpolateColor(p0.y, p0.hue, p2.y, p2.hue);

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

  for (int y = p0.y; y < p2.y; ++y) {
    int fromX = *(int *)getStackItem(xLeft, y - p0.y);
    int toX = *(int *)getStackItem(xRight, y - p0.y);
    Stack *hueSegment =
        interpolateColor(fromX, *(float *)getStackItem(hLeft, y - p0.y), toX,
                         *(float *)getStackItem(hRight, y - p0.y));
    for (int x = fromX; x <= toX; ++x) {
      Color shadedColor =
          shadeColor(color, *(float *)getStackItem(hueSegment, x - fromX));
      setPixel(renderer, (Point2d){x, y}, &shadedColor);
    }
  }

  deleteStack(x012);
  deleteStack(x02);
}

int main(void) {
  printf("Render width: %d\n", RENDER_WIDTH);

  // initialize SDL

  SDL_Event event;
  SDL_Window *window;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window,
                              &renderer);
  //  clearCanva(renderer);
  SDL_SetWindowTitle(window, "window");

  // render the scene

  Color color = {20, 20, 20};
  Color colorTriangle = {255, 255, 255};
  // drawLine((Point2d){0, 0}, (Point2d){0, 200}, &color);
  // drawLine((Point2d){0, 0}, (Point2d){-240, -20}, &color);
  // drawLine((Point2d){0, 0}, (Point2d){200, 0}, &color);

  // drawLine((Point2d){50, -209}, (Point2d){0, 20}, &color);
  // drawLine((Point2d){-50, -20}, (Point2d){60, 240}, &color);

  while (1) {
    clearCanva(renderer);
    drawWireframeTriangle((Point2d){0, 0}, (Point2d){200, 300},
                          (Point2d){400, 100}, &colorTriangle);

    drawTriangle((Point2d){0, 0}, (Point2d){-300, -420}, (Point2d){120, -600},
                 &color);

    drawShadeTriangle((Point2dCarryHue){0, 0, 1},
                      (Point2dCarryHue){-200, -200, 0.5},
                      (Point2dCarryHue){-200, 300, 0}, &color);

    SDL_RenderPresent(renderer);

    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      break;
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
