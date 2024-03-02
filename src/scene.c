#include "scene.h"
#include "camera.h"
#include "instances.h"
#include "models.h"
#include "shapes.h"
#include "stack.h"
#include <stdlib.h>

Scene *createScene() {
  Scene *scene = malloc(sizeof(Scene));
  scene->instances = createStack(0);
  scene->models = createStack(0);
  return scene;
}

void sceneAddCamera(Scene *scene, Camera *camera) { scene->camera = camera; }
void sceneAddModel(Scene *scene, Shape *model) { push(scene->models, model); }
void sceneAddInstance(Scene *scene, Instance *instance) {
  push(scene->instances, instance);
}

void updateScene(Scene *scene) {
  for (int i = 0; i <= scene->instances->top; i++)
    updateInstance(scene->instances->items[i], scene->camera->pos);
}

void destroyScene(Scene *scene) {
  for (int i = 0; i <= scene->instances->top; ++i)
    destroyInstance(scene->instances->items[i]);
  freeStack(scene->instances);

  for (int i = 0; i <= scene->models->top; ++i)
    destroyShape(scene->models->items[i]);
  freeStack(scene->models);

  destroyCamera(scene->camera);
  free(scene);
}
