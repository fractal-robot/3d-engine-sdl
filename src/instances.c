#include "instances.h"
#include "helper.h"
#include "matrix.h"
#include "stack.h"
#include "transform.h"
#include "vertex.h"
#include <stdbool.h>
#include <stdio.h>

Instance *initInstance(Shape *shape) {
  Instance *instance = malloc(sizeof(Instance));

  instance->model = shape;
  instance->updateInstance = false;
  instance->projected = NULL;
  instance->transformationsMat = NULL;
  instance->boundingSphereRadius = 0.0;

  return instance;
}

void setInstanceTransform(Transform transform, float3d value,
                          Instance *instance) {
  if (transform == SCALE) {
    instance->updateBoundingSphere = true;
    instance->s = value;
  } else if (transform == ROTATE)
    instance->r = value;
  else if (transform == TRANSLATE)
    instance->t = value;

  instance->updateInstance = true;
};

void updateInstanceMatrices(Instance *instance) {
  Mat *translate = createTranslationMatrix(&instance->t);
  Mat *rotate = createRotationMatrix(&instance->r);
  Mat *combinedMat = multiplyMat(translate, rotate);
  freeMat(rotate);
  freeMat(translate);
  Mat *scale = createScaleMatrix(&instance->s);
  instance->transformationsMat = multiplyMat(combinedMat, scale);
  freeMat(combinedMat);
  freeMat(scale);
}

void updateBoundingSphereRadius(Instance *instance) {
  int verticesCount = instance->model->verticesCount;
  Mat *vertexPos = createMat(4, 1, false);
  setW(vertexPos, 1);

  for (int v = 0; v < verticesCount; ++v) {
    setX(vertexPos, instance->model->vertices[v].x);
    setY(vertexPos, instance->model->vertices[v].y);
    setZ(vertexPos, instance->model->vertices[v].z);

    Mat *transformed = multiplyMat(instance->transformationsMat, vertexPos);

    float currentBoundingSphereRadius =
        distanceToCenter(instance->t, transformed);
    if (currentBoundingSphereRadius > instance->boundingSphereRadius)
      instance->boundingSphereRadius = currentBoundingSphereRadius;

    freeMat(transformed);
  }

  freeMat(vertexPos);
}

void updateInstanceVertices(Instance *instance) {
  int verticesCount = instance->model->verticesCount;
  instance->projected = createStack(verticesCount);

  Mat *vertexPos = createMat(4, 1, false);
  setW(vertexPos, 1);

  for (int v = 0; v < verticesCount; ++v) {
    Vertex *vertex = createVertex();
    vertex->position = createMat(4, 1, false);

    setX(vertexPos, instance->model->vertices[v].x);
    setY(vertexPos, instance->model->vertices[v].y);
    setZ(vertexPos, instance->model->vertices[v].z);

    vertex->position = multiplyMat(instance->transformationsMat, vertexPos);
    push(instance->projected, vertex);
  }

  freeMat(vertexPos);
}

void updateInstance(Instance *instance, const Mat *cameraPos) {
  instance->tc.x = instance->t.x - getX(cameraPos);
  instance->tc.y = instance->t.y - getY(cameraPos);
  instance->tc.z = instance->t.z - getZ(cameraPos);

  if (instance->updateInstance == false)
    return;

  if (instance->transformationsMat != NULL) {
    freeMat(instance->transformationsMat);
    instance->transformationsMat = NULL;
  }

  if (instance->projected != NULL) {
    for (int i = 0; i < instance->projected->top; ++i)
      destroyVertex(instance->projected->items[i]);
    freeStack(instance->projected);
    instance->projected = NULL;
  }

  updateInstanceMatrices(instance);
  updateInstanceVertices(instance);

  instance->updateInstance = false;

  if (instance->updateBoundingSphere == true ||
      instance->boundingSphereRadius == 0.0) {
    instance->updateBoundingSphere = false;
    instance->boundingSphereRadius = 0.0;
    updateBoundingSphereRadius(instance);
  }
}

void destroyInstance(Instance *instance) {
  freeMat(instance->transformationsMat);
  for (int i = 0; i < instance->projected->top; ++i)
    destroyVertex(instance->projected->items[i]);
  freeStack(instance->projected);
  free(instance);
}

void printInstanceDescription(Instance *instance) {
  printf("[DEBUG] Instance model: %p.\n", instance->model);
  printf("[DEBUG] Instance %p bounding sphere radius: %f.\n", &instance,
         instance->boundingSphereRadius);
}
