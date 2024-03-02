#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "instances.h"
#include "shapes.h"
#include "stack.h"

typedef struct {
  Stack *instances;
  Stack *models; // we need to store the models of the instances in order to be
                 // able to free them once the scene have been destroyed
  Stack *validInstances;     // instances that are in the clipping frustrum
  Stack *correctedInstances; // instances that crossed the border of the
                             // clipping planes
  Camera *camera;
} Scene;

Scene *createScene();
void sceneAddCamera(Scene *scene, Camera *camera);
void sceneAddModel(Scene *scene, Shape *model);
void sceneAddInstance(Scene *scene, Instance *instance);
void destroyScene(Scene *scene);
void renderScene(Scene *scene);

#endif
