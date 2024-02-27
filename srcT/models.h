#ifndef MODELS_H
#define MODELS_H

#include "structs.h"
#include <SDL2/SDL_render.h>

typedef struct {
  Triangle *trianglesList;
  Mat3d *vertices;
  int trianglesCount;
  int verticesCount;
} Model;

Model *createModelCube();
void renderModel(SDL_Renderer *renderer, Model *model);

#endif
