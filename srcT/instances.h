#ifndef INSTANCES_C
#define INSTANCES_C

#include "models.h"
#include "transform.h"

typedef struct {
  Model *model;
  Scale scale;
  Translate translate;

} Instance;

void renderInstance(SDL_Renderer *renderer, Instance *instance);

#endif
