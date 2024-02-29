#include "instances.h"
#include "matrix.h"
#include "planes.h"
#include "scene.h"
#include "stack.h"

float signedDistance(Plane *plane, point3d *vertex) {
  Mat *normal = getPlaneNormal(plane);
  return (vertex->data[0][0] * normal[0][0]) + (vertex[1][0] * normal[1][0]) +
         (vertex[2][0] * normal[2][0]) + plane->distance;
}

Instance clipInstanceAgainstPlanes(Instance *instance, Plane *plane) {
  float d = signedDistance(plane, instance->sphere.center);
  if (d > r)
    return instance;
  else if (d < -r)
    return NULL;
  else {
    Instance clippedInstance;
  }
};

Instance *clipInstance(Instance *instance, float planes) {
  for (int p = 0; p < 5; ++p) { // for each plane
    Instance *instance = clipInstanceAgainstPlane(instance, plane);
  }
}

Scene clipScene(Scene *scene, float planes) {
  Stack *clippedInstances = createStack(0);

  for (int i = 0; i <= scene->instances->top; ++i) {
  }
}
