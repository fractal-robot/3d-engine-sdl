#include "scene.h"
#include "stack.h"
#include <stdlib.h>

Scene *createScene() {
  Scene *scene = malloc(sizeof(Scene));
  scene->instances = createStack(4);
  return scene;
}
