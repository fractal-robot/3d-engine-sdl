#ifndef SCENE_H
#define SCENE_H

#include "stack.h"

typedef struct {
  Stack *instances;
} Scene;

Scene *createScene();

#endif
