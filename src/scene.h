#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "instances.h"
#include "shapes.h"
#include "stack.h"

typedef struct {
  Stack *instances;
  Stack *newInstances; // pointer to all the instances that are totally in the
                       // inside plane of the clipping space
  Stack *newTriangles; // set of the new triangles that have been calculated on
                       // the second step of the clipping
  Stack *models; // we need to store the models of the instances in order to be

  // able to free them once the scene have been destroyed
  Camera *camera;
} Scene;

Scene *createScene();
void sceneAddCamera(Scene *scene, Camera *camera);
void sceneAddModel(Scene *scene, Shape *model);
void sceneAddInstance(Scene *scene, Instance *instance);
void destroyScene(Scene *scene);
void renderScene(Scene *scene);

#endif
