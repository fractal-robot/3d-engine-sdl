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

float signedDistance(const Plane *plane, const float3d *vertex) {
  return (vertex->x * plane->x) + (vertex->y * plane->y) +
         (vertex->z * plane->z) + plane->d;
}

Instance *clipInstanceAgainstPlane(Instance *instance, Plane *plane) {
  float d = signedDistance(plane, &instance->t);
  printf("Distance from the plane: %f\n", d);
  if (d > instance->boundingSphereRadius) {
    return instance;
  }
  return NULL;
}

Instance *clipInstance(Instance *instance) {
  Plane *planes[5] = {&nearPlane, &leftPlane, &rightPlane, &bottomPlane,
                      &topPlane};
  for (int i = 0; i < 5; ++i) {
    instance = clipInstanceAgainstPlane(instance, planes[i]);
    if (instance == NULL)
      return NULL;
  }
  return instance;
}

/*
Instance *clipInstance(Instance *instance) {
  Plane *planes[5] = {&nearPlane, &leftPlane, &rightPlane, &bottomPlane,
                      &topPlane};

  Instance *newInstance = malloc(sizeof(Instance));

  memcpy(newInstance, instance, sizeof(Instance));

  return newInstance;
} */

Stack *clipScene(Scene *scene) {
  Stack *clippedInstances = createStack(4);

  for (int i = 0; i <= scene->instances->top; ++i) {
    Instance *newInstance = clipInstance(scene->instances->items[i]);
    if (newInstance != NULL)
      push(clippedInstances, newInstance);
  }

  printf("\n\n\n\n\n");

  return clippedInstances;
}
