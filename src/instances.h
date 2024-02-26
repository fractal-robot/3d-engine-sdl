#ifndef INSTANCES_C
#define INSTANCES_C

#include "models.h"
#include "structs.h"

typedef struct {
  Model *model;
  Mat3d translation; // make sure to make the translation last
  float scale;

} Instance;

void renderInstance(SDL_Renderer *renderer, Instance *instance);

#endif
