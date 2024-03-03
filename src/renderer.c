#include "camera.h"
#include "clipping.h"
#include "definition.h"
#include "helper.h"
#include "instances.h"
#include "matrix.h"
#include "scene.h"
#include "sdl-interface.h"
#include "stack.h"
#include "vertex.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

float zBuffer[RENDER_WIDTH][RENDER_WIDTH];

void resetZBuffer() {
  for (int row = 0; row < RENDER_WIDTH; ++row)
    for (int col = 0; col < RENDER_WIDTH; ++col)
      zBuffer[row][col] = INFINITY;
}

bool testZBuffer(int x, int y, float z) {
  const int pX = (int)(RENDER_WIDTH / 2) + x;
  const int pY = (int)(RENDER_WIDTH / 2) - y;

  if (x < -RENDER_WIDTH / 2 || x > RENDER_WIDTH / 2 || y < -RENDER_WIDTH / 2 ||
      y > RENDER_WIDTH / 2) {
    return false;
  }

  if (z < zBuffer[pX][pY]) {
    zBuffer[pX][pY] = z;
    return true;
  }

  return false;
}

void renderInstance(Instance *instance, Camera *camera) {
  Vertex *currentVertex;

  for (int t = 0; t < instance->model->trianglesCount; ++t) {
    // project the 3 vertex of the triangle to screen coordinates and keep Z for
    // the depth buffer

    Triangle triangle = instance->model->trianglesList[t];

    // instance transformations
    currentVertex = instance->projected->items[triangle.a];
    Mat *v0 = multiplyMat(camera->lookAt, currentVertex->position);

    currentVertex = instance->projected->items[triangle.b];
    Mat *v1 = multiplyMat(camera->lookAt, currentVertex->position);

    currentVertex = instance->projected->items[triangle.c];
    Mat *v2 = multiplyMat(camera->lookAt, currentVertex->position);

    // projection to screen coordinates
    Mat *p0 = projectVertexCarryZ(v0);
    Mat *p1 = projectVertexCarryZ(v1);
    Mat *p2 = projectVertexCarryZ(v2);

    // sort the vertex based on Y coordinates
    if (getY(p1) < getY(p0))
      swapMatPointers(&p0, &p1);
    if (getY(p2) < getY(p0))
      swapMatPointers(&p0, &p2);
    if (getY(p2) < getY(p1))
      swapMatPointers(&p1, &p2);

    // interpolate to get the X values of the sides
    Stack *x012 = interpolate(getY(p0), getX(p0), getY(p1), getX(p1));
    pop(x012);
    Stack *x12 = interpolate(getY(p1), getX(p1), getY(p2), getX(p2));
    append(x012, x12);
    Stack *x02 = interpolate(getY(p0), getX(p0), getY(p2), getX(p2));

    // interpolate to get the Z values of the sides
    Stack *z012 = interpolateFloat(getY(p0), getZ(p0), getY(p1), getZ(p1));
    pop(z012);
    Stack *z12 = interpolateFloat(getY(p1), getZ(p1), getY(p2), getZ(p2));
    append(z012, z12);
    Stack *z02 = interpolateFloat(getY(p0), getZ(p0), getY(p2), getZ(p2));

    Stack *xLeft;
    Stack *zLeft;
    Stack *xRight;
    Stack *zRight;

    // sort the two side, right and left
    int m = floor((float)x02->top / 2);
    if (*(int *)getStackItem(x02, m) < *(int *)getStackItem(x012, m)) {
      xLeft = x02;
      zLeft = z02;
      xRight = x012;
      zRight = z012;
    } else {
      xLeft = x012;
      zLeft = z012;
      xRight = x02;
      zRight = z02;
    }

    Mat *pixelCoord = createMat(2, 1, false);

    for (int y = getY(p0); y < getY(p2); ++y) {
      int fromX = *(int *)getStackItem(xLeft, y - getY(p0));
      int toX = *(int *)getStackItem(xRight, y - getY(p0));

      Stack *zSegment =
          interpolateFloat(fromX, *(float *)getStackItem(zLeft, y - getY(p0)),
                           toX, *(float *)getStackItem(zRight, y - getY(p0)));

      for (int x = fromX; x < toX; ++x) {
        float zVal = *(float *)getStackItem(zSegment, x - fromX);

        if (testZBuffer(x, y, zVal)) {
          setX(pixelCoord, x);
          setY(pixelCoord, y);
          setPixel(pixelCoord, &triangle.color);
        }
      }
    }

    for (int i = 0; i <= x012->top; ++i)
      free(x012->items[i]);
    freeStack(x012);

    for (int i = 0; i <= x02->top; ++i)
      free(x02->items[i]);
    freeStack(x02);

    for (int i = 0; i <= z012->top; ++i)
      free(z012->items[i]);
    freeStack(z012);

    for (int i = 0; i <= z02->top; ++i)
      free(z02->items[i]);
    freeStack(z02);

    freeMat(v0);
    freeMat(v1);
    freeMat(v2);

    freeMat(p0);
    freeMat(p1);
    freeMat(p2);
  }
}

void renderWireframeInstance(Instance *instance, Camera *camera) {
  Vertex *currentVertex;

  for (int t = 0; t < instance->model->trianglesCount; ++t) {
    Triangle triangle = instance->model->trianglesList[t];

    currentVertex = instance->projected->items[triangle.a];
    Mat *v1 = multiplyMat(camera->lookAt, currentVertex->position);

    currentVertex = instance->projected->items[triangle.b];
    Mat *v2 = multiplyMat(camera->lookAt, currentVertex->position);

    currentVertex = instance->projected->items[triangle.c];
    Mat *v3 = multiplyMat(camera->lookAt, currentVertex->position);

    Mat *p1 = projectVertex(v1);
    Mat *p2 = projectVertex(v2);
    Mat *p3 = projectVertex(v3);

    drawWireframeTriangle(p1, p2, p3, &triangle.color);

    freeMat(v1);
    freeMat(v2);
    freeMat(v3);

    freeMat(p1);
    freeMat(p2);
    freeMat(p3);
  }
}

void renderScene(Scene *scene) {
  resetZBuffer();
  for (int i = 0; i <= scene->instances->top; ++i) {
    Instance *currentInstance = clipInstance(scene->instances->items[i]);
    if (currentInstance != NULL)
      renderInstance(currentInstance, scene->camera);
  }
}
