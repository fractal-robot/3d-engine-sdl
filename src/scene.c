#include "scene.h"
#include "camera.h"
#include "clipping.h"
#include "instances.h"
#include "models.h"
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

void destroyScene(Scene *scene) {
  for (int i = 0; i <= scene->models->top; ++i)
    free(scene->models->items[i]);
  freeStack(scene->models);

  for (int i = 0; i <= scene->instances->top; ++i)
    free(scene->instances->items[i]);
  freeStack(scene->instances);

  free(scene);
}

void renderScene(Scene *scene) {
  for (int i = 0; i <= scene->instances->top; i++)
    updateInstance(scene->instances->items[i], scene->camera);

  scene->newInstances = clipScene(scene);

  for (int i = 0; i <= scene->newInstances->top; i++)
    renderInstance(scene->newInstances->items[i]);
}
