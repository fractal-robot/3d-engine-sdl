#ifndef INSTANCES_C
#define INSTANCES_C

#include "camera.h"
#include "models.h"
#include "stack.h"
#include "structs.h"

typedef struct {
  Model *model;
  Stack *projected;
  float3d s;                  // scaling
  float3d t;                  // translation
  float3d r;                  // rotation (right-handed)
  float3d pv;                 // previous translations
  float boundingSphereRadius; // the center is t;
} Instance;

void calculateInstanceBoundingSphere(Instance *instance);
void updateInstance(Instance *instance, Camera *camera);
void renderInstance(SDL_Renderer *renderer, Instance *instance);

void renderInstances(SDL_Renderer *renderer, Stack *instances);
void updateInstances(Camera *camera, Stack *instances);

#endif
