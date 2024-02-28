#include "instances.h"
#include "camera.h"
#include "helper.h"
#include "matrix.h"
#include "stack.h"
#include "structs.h"
#include "transform.h"
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void renderInstance(SDL_Renderer *renderer, Instance *instance,
                    Camera *camera) {
  if (instance->model == NULL) {
    printf("[ERROR] Instance does not point to any object.");
    exit(EXIT_FAILURE);
  }

  int verticesCount = instance->model->verticesCount;
  Stack *projected = createStack(verticesCount);

  for (int v = 0; v < verticesCount; ++v) {
    Mat *vertexPos = createMat(4, 1, false);
    vertexPos->data[0][0] = instance->model->vertices[v].x;
    vertexPos->data[1][0] = instance->model->vertices[v].y;
    vertexPos->data[2][0] = instance->model->vertices[v].z;
    vertexPos->data[3][0] = 1;

    vertexPos = rotate(vertexPos, &instance->r);
    vertexPos = scale(vertexPos, &instance->s);
    translate(vertexPos, &instance->t); // will change vertexPos

    vertexPos = multiplyMat(camera->lookAt, vertexPos);

    vertexPos = projectVertex(vertexPos);

    vertexPos->rows = 2; // review code
    vertexPos->cols = 1;

    push(projected, vertexPos);
  }

  for (int t = 0; t < instance->model->trianglesCount; ++t) {
    Triangle triangle = instance->model->trianglesList[t];
    Mat *v1 = getStackItem(projected, triangle.a);
    Mat *v2 = getStackItem(projected, triangle.b);
    Mat *v3 = getStackItem(projected, triangle.c);
    drawWireframeTriangle(renderer, v1, v2, v3, &triangle.color);
  }
}
