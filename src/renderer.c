#include "camera.h"
#include "clipping.h"
#include "helper.h"
#include "instances.h"
#include "scene.h"
#include "vertex.h"

void renderWireframeInstance(Instance *instance, Camera *camera) {
  Vertex *currentVertex;

  for (int t = 0; t < instance->model->trianglesCount; ++t) {
    Triangle triangle = instance->model->trianglesList[t];

    currentVertex = instance->projected->items[triangle.a];
    Mat *v1 = multiplyMat(camera->lookAt, currentVertex->position);

    currentVertex = instance->projected->items[triangle.b];
    Mat *v2 = multiplyMat(camera->lookAt, currentVertex->position);

    currentVertex = instance->projected->items[triangle.c];
    Mat *v3 = multiplyMat(camera->lookAt, currentVertex->position);

    Mat *p1 = projectVertex(v1);
    Mat *p2 = projectVertex(v2);
    Mat *p3 = projectVertex(v3);

    Color color = {255, 255, 255};
    drawWireframeTriangle(p1, p2, p3, &color);

    freeMat(v1);
    freeMat(v2);
    freeMat(v3);

    freeMat(p1);
    freeMat(p2);
    freeMat(p3);
  }
}

void renderScene(Scene *scene) {
  for (int i = 0; i <= scene->instances->top; ++i) {
    Instance *currentInstance = clipInstance(scene->instances->items[i]);
    if (currentInstance != NULL)
      renderWireframeInstance(currentInstance, scene->camera);
  }
}
