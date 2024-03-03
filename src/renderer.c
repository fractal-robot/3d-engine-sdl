#include "camera.h"
#include "clipping.h"
#include "definition.h"
#include "helper.h"
#include "instances.h"
#include "matrix.h"
#include "scene.h"
#include "stack.h"
#include "vertex.h"
#include <math.h>
#include <stdio.h>

float zBuffer[RENDER_WIDTH - 1][RENDER_WIDTH - 1];

void resetZBuffer() {
  for (int row = 0; row < RENDER_WIDTH; ++row)
    for (int col = 0; col < RENDER_WIDTH; ++col)
      zBuffer[row][col] = INFINITY;
}

/*
void interpretZBuffer(Mat *p1, Mat *p2, Mat *p3, float z1, float z2, float z3) {
  float temp;

  // sort the vertex by Y value
  if (getY(p2) < getY(p1)) {
    swapMatPointers(&p1, &p2);
    temp = z1;
    z1 = z2;
    z2 = temp;
  }
  if (getY(p3) < getY(p1)) {
    swapMatPointers(&p3, &p1);
    temp = z1;
    z1 = z3;
    z3 = temp;
  }
  if (getY(p3) < getY(p2)) {
    swapMatPointers(&p3, &p2);
    temp = z2;
    z2 = z3;
    z3 = temp;
  }

  Stack *z012 = interpolate(getY(p1), z1, getY(p2), z2);
  pop(z012);
  Stack *z12 = interpolate(getY(p2), z2, getY(p3), z3);
  append(z012, z12);

  Stack *z02 = interpolate(getY(p1), z1, getY(p3), z3);

  Stack *zLeft;
  Stack *zRight;

  int m = floor((float)z02->top / 2);
  if (*(int *)getStackItem(z02, m) < *(int *)getStackItem(z012, m)) {
    zLeft = z02;
    zRight = z012;
  } else {
    zLeft = z012;
    zRight = z02;
  }

  for (int y = getY(p1); y < getY(p3); ++y) {
    int fromZ = *(int *)getStackItem(zLeft, y - getY(p1));
    int toZ = *(int *)getStackItem(zRight, y - getY(p1));

    for (int z = fromZ; z <= toZ; ++z) {
    }
  }
}

void renderInstance(Instance *instance, Camera *camera) {
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

    drawTriangle(p1, p2, p3, &triangle.color);

    freeMat(v1);
    freeMat(v2);
    freeMat(v3);

    freeMat(p1);
    freeMat(p2);
    freeMat(p3);
  }
}
*/

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

    Color color = {255, 255, 255};
    drawWireframeTriangle(p1, p2, p3, &color);

    freeMat(v1);
    freeMat(v2);
    freeMat(v3);

    freeMat(p1);
    freeMat(p2);
    freeMat(p3);
  }
}

void renderScene(Scene *scene) {
  for (int i = 0; i <= scene->instances->top; ++i) {
    Instance *currentInstance = clipInstance(scene->instances->items[i]);
    if (currentInstance != NULL)
      renderWireframeInstance(currentInstance, scene->camera);
  }
}
