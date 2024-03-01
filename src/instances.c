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

void updateInstances(Camera *camera, Stack *instances) {
  for (int i = 0; i <= instances->top; ++i)
    updateInstance(getStackItem(instances, i), camera);
}

void calculateInstanceBoundingSphere(Instance *instance) {
  float maxValue = 0.0;
  int verticesCount = instance->model->verticesCount;
  float3d pointPos;

  for (int v = 0; v < verticesCount; ++v) {
    Mat *vertexPos = createMat(4, 1, false);
    setX(vertexPos, instance->model->vertices[v].x);
    setY(vertexPos, instance->model->vertices[v].y);
    setZ(vertexPos, instance->model->vertices[v].z);
    setW(vertexPos, 1);

    vertexPos = scale(vertexPos, &instance->s);

    pointPos.x = getX(vertexPos);
    pointPos.y = getY(vertexPos);
    pointPos.z = getZ(vertexPos);

    float currentValue = findDistanceToCenter(&pointPos);

    if (currentValue > maxValue)
      maxValue = currentValue;
  }

  instance->boundingSphereRadius = maxValue;
  printf("[DEBUG] Instance bounding sphere radius: %f.\n", maxValue);
}

void updateInstance(Instance *instance, Camera *camera) {
  if (instance->model == NULL) {
    printf("[ERROR] Instance does not point to any object.");
    exit(EXIT_FAILURE);
  }

  int verticesCount = instance->model->verticesCount;
  instance->projected = createStack(verticesCount);

  for (int v = 0; v < verticesCount; ++v) {
    Mat *vertexPos = createMat(4, 1, false);
    setX(vertexPos, instance->model->vertices[v].x);
    setY(vertexPos, instance->model->vertices[v].y);
    setZ(vertexPos, instance->model->vertices[v].z);
    setW(vertexPos, 1);

    vertexPos = scale(vertexPos, &instance->s);
    vertexPos = rotate(vertexPos, &instance->r);
    translate(vertexPos, &instance->t); // will change vertexPos

    vertexPos = multiplyMat(camera->lookAt, vertexPos);

    // update object data such that clipping works.
    instance->t.x -= instance->pv.x + getX(camera->pos);
    instance->t.y -= instance->pv.y + getY(camera->pos);
    instance->t.z -= instance->pv.z + getZ(camera->pos);

    Mat *projectedPoint = projectVertex(vertexPos);
    freeMat(vertexPos);

    push(instance->projected, projectedPoint);

    instance->pv = instance->t;
  }
}

void renderInstance(Instance *instance) {
  for (int t = 0; t < instance->model->trianglesCount; ++t) {
    Triangle triangle = instance->model->trianglesList[t];
    Mat *v1 = getStackItem(instance->projected, triangle.a);
    Mat *v2 = getStackItem(instance->projected, triangle.b);
    Mat *v3 = getStackItem(instance->projected, triangle.c);
    Color color = {255, 255, 255};
    drawWireframeTriangle(v1, v2, v3, &color);
  }
}
