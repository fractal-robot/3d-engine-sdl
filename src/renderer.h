#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "instances.h"
#include "scene.h"

void renderInstance(Instance *instance, Camera *camera);
void renderWireframeInstance(Instance *instance, Camera *camera);
void renderScene(Scene *scene);

#endif
