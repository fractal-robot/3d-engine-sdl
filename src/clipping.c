#include "definition.h"
#include "instances.h"
#include "matrix.h"
#include "scene.h"
#include "stack.h"
#include "structs.h"
#include "vertex.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SQRT2 1.414213562 // Precomputed value of sqrt(2)
#define PLANE_DISTANCE

typedef struct {
  float x;
  float y;
  float z;
  int d;
} Plane;

// normals of the 5 clipping planes, d is the negative signed distance from the
// origin to the plane P
Plane nearPlane = (Plane){0, 0, 1, D};
Plane leftPlane = (Plane){1 / SQRT2, 0, 1 / SQRT2, 0};
Plane rightPlane = (Plane){-1 / SQRT2, 0, 1 / SQRT2, 0};
Plane bottomPlane = (Plane){0, 1 / SQRT2, 1 / SQRT2, 0};
Plane topPlane = (Plane){0, -1 / SQRT2, 1 / SQRT2, 0};

float signedDistance(const Plane *plane, const float3d *vertex) {
  return (vertex->x * plane->x) + (vertex->y * plane->y) +
         (vertex->z * plane->z) + plane->d;
}

/*
Stack *clipTriangle(Vertex *v1, Vertex *v2, Vertex *v3, Plane *plane) {
  Stack *clippedTriangle = createStack(3);

  float d1 = signedDistance(plane, v1);
  float d2 = signedDistance(plane, v2);
  float d3 = signedDistance(plane, v3);

  if (d1 > 0 && d2 > 0 && d3 > 0) {
    push(clippedTriangle, v1);
    push(clippedTriangle, v2);
    push(clippedTriangle, v3);
    return clippedTriangle;
  } else if (d1 <= 0 && d2 <= 0 && d3 <= 0) {
    return NULL;
  }
}

clipTrianglesAgainstPlane(Instance *instance, Plane *plane) {
  Stack *clippedTriangles = createStack(1);
  Mat *triangleVertices = createMat(3, 1, false);

  for (int i = 0; i < instance->model->trianglesCount; ++i) {
    const Triangle triangle = instance->model->trianglesList[i];

    Vertex *v1 = instance->projected->items[triangle.a];
    Vertex *v2 = instance->projected->items[triangle.a];
    Vertex *v3 = instance->projected->items[triangle.a];

    push(clippedTriangles, clipTriangle(instance->model.))
  }

  freeMat(triangleVertices);
}
*/

Instance *clipInstanceAgainstPlane(Instance *instance, Plane *plane) {
  float d = signedDistance(plane, &instance->tc);
  if (d > instance->boundingSphereRadius) {
    return instance;
  } else if (d < -instance->boundingSphereRadius) {
    return NULL;
  } else {
    return NULL;
  }
}

Instance *clipInstance(Instance *instance) {
  Plane *planes[5] = {&nearPlane, &leftPlane, &rightPlane, &bottomPlane,
                      &topPlane};
  Instance *newInstance;
  for (int i = 0; i < 5; ++i) {
    newInstance = clipInstanceAgainstPlane(instance, planes[i]);
    if (newInstance == NULL)
      return NULL;
  }
  return newInstance;
}

void clipScene(Scene *scene) {
  scene->validInstances = createStack(1);

  for (int i = 0; i <= scene->instances->top; ++i) {
    Instance *newInstance = clipInstance(scene->instances->items[i]);
    if (newInstance != NULL)
      push(scene->validInstances, newInstance);
  }
}
