#ifndef INSTANCES_C
#define INSTANCES_C

#include "camera.h"
#include "models.h"
#include "transform.h"

typedef struct {
  Model *model;
  float3d s; // scaling
  float3d t; // translation
  float3d r; // rotation (right-handed)

} Instance;

void renderInstance(SDL_Renderer *renderer, Instance *instance, Camera *camera);

#endif
