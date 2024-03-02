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

/* void calculateInstanceBoundingSphere(Instance *instance) {
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
 printf("[DEBUG] Instance %p bounding sphere radius: %f.\n", &instance,
        maxValue);
}
*/

Instance *initInstance(Shape *shape) {
  Instance *instance = malloc(sizeof(Instance));

  instance->model = shape;
  instance->updateInstance = false;
  instance->projected = NULL;
  instance->transformationsMat = NULL;

  return instance;
}

void renderWireframeInstance(Instance *instance, Camera *camera) {
  for (int t = 0; t < instance->model->trianglesCount; ++t) {
    Triangle triangle = instance->model->trianglesList[t];

    Mat *v1 = multiplyMat(camera->lookAt,
                          getStackItem(instance->projected, triangle.a));
    Mat *v2 = multiplyMat(camera->lookAt,
                          getStackItem(instance->projected, triangle.b));
    Mat *v3 = multiplyMat(camera->lookAt,
                          getStackItem(instance->projected, triangle.c));

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

void setInstanceTransform(Transform transform, float3d value,
                          Instance *instance) {
  if (transform == SCALE)
    instance->s = value;
  else if (transform == ROTATION)
    instance->r = value;
  else if (transform == TRANSLATION)
    instance->t = value;

  instance->updateInstance = true;
};

void updateInstanceMatrices(Instance *instance) {
  Mat *translate = createTranslationMatrix(&instance->t);
  Mat *rotate = createRotationMatrix(&instance->r);
  Mat *scale = createScaleMatrix(&instance->s);

  Mat *combinedMat = multiplyMat(translate, rotate);
  instance->transformationsMat = multiplyMat(combinedMat, scale);

  freeMat(combinedMat);
  freeMat(scale);
  freeMat(rotate);
  freeMat(translate);
}

void updateInstanceVertices(Instance *instance) {
  int verticesCount = instance->model->verticesCount;
  instance->projected = createStack(verticesCount);

  Mat *vertexPos = createMat(4, 1, false);

  for (int v = 0; v < verticesCount; ++v) {
    setX(vertexPos, instance->model->vertices[v].x);
    setY(vertexPos, instance->model->vertices[v].y);
    setZ(vertexPos, instance->model->vertices[v].z);
    setW(vertexPos, 1);

    Mat *transformedPoint =
        multiplyMat(instance->transformationsMat, vertexPos);
    push(instance->projected, transformedPoint);
  }

  freeMat(vertexPos);
}

void updateInstance(Instance *instance) {
  if (instance->updateInstance == false)
    return;

  if (instance->transformationsMat != NULL) {
    freeMat(instance->transformationsMat);
    instance->transformationsMat = NULL;
  }
  if (instance->projected != NULL) {
    freeStack(instance->projected);
    instance->projected = NULL;
  }

  updateInstanceMatrices(instance);
  updateInstanceVertices(instance);

  instance->updateInstance = false;
}

void destroyInstance(Instance *instance) {
  freeMat(instance->transformationsMat);
  for (int i = 0; i < instance->projected->top; ++i)
    freeMat(instance->projected->items[i]);
  freeStack(instance->projected);
  free(instance);
}
