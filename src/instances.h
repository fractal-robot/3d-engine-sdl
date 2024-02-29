#ifndef INSTANCES_C
#define INSTANCES_C

#include "camera.h"
#include "models.h"
#include "stack.h"
#include "structs.h"

typedef struct {
  Model *model;
  float3d s;                  // scaling
  float3d t;                  // translation
  float3d r;                  // rotation (right-handed)
  float boundingSphereRadius; // the center is t;
} Instance;

Model *initInstance(Model *model);
void renderInstances(SDL_Renderer *renderer, Stack *instances, Camera *camera);
void renderInstance(SDL_Renderer *renderer, Instance *instance, Camera *camera);
void calculateInstanceBoundingSphere(Instance *instance);

#endif
