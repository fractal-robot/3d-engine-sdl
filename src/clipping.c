#include "definition.h"
#include "instances.h"
#include "scene.h"
#include "stack.h"
#include "structs.h"
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

// clipTrianglesAgainstPlane() { Stack *clippedTriangles = createStack(1); }

float signedDistance(const Plane *plane, const float3d *vertex) {
  return (vertex->x * plane->x) + (vertex->y * plane->y) +
         (vertex->z * plane->z) + plane->d;
}

Instance *clipInstanceAgainstPlane(Instance *instance, Plane *plane) {
  float d = signedDistance(plane, &instance->tc);
  if (d > instance->boundingSphereRadius) {
    return instance;
  } else if (d < -instance->boundingSphereRadius) {
    return NULL;
  } else {
    //    clipTrianglesAgainstPlane(instance);
    return NULL;
  }
}

Instance *clipInstance(Instance *instance) {
  Plane *planes[5] = {&nearPlane, &leftPlane, &rightPlane, &bottomPlane,
                      &topPlane};
  Instance *newInstance;
  for (int i = 0; i < 5; ++i) {
    newInstance = clipInstanceAgainstPlane(instance, planes[i]);
    if (instance == NULL)
      return NULL;
  }
  return newInstance;
}

/*
Instance *clipInstance(Instance *instance) {
  Plane *planes[5] = {&nearPlane, &leftPlane, &rightPlane, &bottomPlane,
                      &topPlane};

  Instance *newInstance = malloc(sizeof(Instance));

  memcpy(newInstance, instance, sizeof(Instance));

  return newInstance;
} */

void clipScene(Scene *scene) {
  scene->validInstances = createStack(1);

  for (int i = 0; i <= scene->instances->top; ++i) {
    Instance *newInstance = clipInstance(scene->instances->items[i]);
    if (newInstance != NULL)
      push(scene->validInstances, newInstance);
  }
}
