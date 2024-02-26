#include "instances.h"
#include "helper.h"
#include "stack.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

void renderInstance(SDL_Renderer *renderer, Instance *instance) {
  if (instance->model == NULL) {
    printf("[ERROR] An instance does not point to any object.");
    exit(EXIT_FAILURE);
  }

  int verticesCount = instance->model->verticesCount;
  Stack *projected = createStack(verticesCount);

  for (int v = 0; v < verticesCount; ++v) {
    Mat3d currentVertex = translate(instance->model->vertices[v]);
    Mat2d vertex = projectVertex(&currentVertex);
    pushLocal(projected, &vertex, sizeof(vertex));
  }

  for (int t = 0; t < instance->model->trianglesCount; ++t) {
    Triangle triangle = instance->model->trianglesList[t];
    Mat2d v1 = *(Mat2d *)getStackItem(projected, triangle.a);
    Mat2d v2 = *(Mat2d *)getStackItem(projected, triangle.b);
    Mat2d v3 = *(Mat2d *)getStackItem(projected, triangle.c);
    drawWireframeTriangle(renderer, v1, v2, v3, &triangle.color);
  }
}
