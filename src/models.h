#ifndef MODELS_H
#define MODELS_H

#include "structs.h"
#include <SDL2/SDL_render.h>

typedef struct {
  Triangle *trianglesList;
  float3d *vertices;
  int trianglesCount;
  int verticesCount;
} Model;

Model *createModelCube();

#endif
