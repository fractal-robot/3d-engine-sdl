#ifndef INSTANCES_C
#define INSTANCES_C

#include "camera.h"
#include "shapes.h"
#include "stack.h"
#include "structs.h"
#include <stdbool.h>

typedef enum : char {
  SCALE,
  ROTATION,
  TRANSLATION,
} Transform;

typedef struct {
  Shape *model;
  Stack *projected;
  _Bool updateInstance;
  float3d s; // scaling
  float3d t; // translation
  float3d r; // rotation (right-handed)
  Mat *transformationsMat;
  float boundingSphereRadius; // the center is t;
} Instance;

void renderWireframeInstance(Instance *instance, Camera *camera);
void setInstanceTransform(Transform transform, float3d value,
                          Instance *instance);
void updateInstance(Instance *instance);
Instance *initInstance(Shape *shape);
void destroyInstance(Instance *instance);

#endif
