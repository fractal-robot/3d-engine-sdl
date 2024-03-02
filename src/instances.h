#ifndef INSTANCES_C
#define INSTANCES_C

#include "matrix.h"
#include "shapes.h"
#include "stack.h"
#include "structs.h"
#include <stdbool.h>

typedef enum : char {
  SCALE,
  ROTATE,
  TRANSLATE,
} Transform;

typedef struct {
  Shape *model;
  Stack *projected;
  bool updateInstance;
  bool updateBoundingSphere;
  float3d s;  // scaling
  float3d t;  // translation
  float3d tc; // translation that takes camera into account, for clipping
  float3d r;  // rotation (right-handed)
  Mat *transformationsMat;
  float boundingSphereRadius; // the center is t;
} Instance;

Instance *initInstance(Shape *shape);
void setInstanceTransform(Transform transform, float3d value,
                          Instance *instance);
void updateInstance(Instance *instance, const Mat *cameraPos);
void destroyInstance(Instance *instance);
void printInstanceDescription(Instance *instance);

#endif
