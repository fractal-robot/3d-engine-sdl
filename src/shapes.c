#include "shapes.h"
#include "definition.h"
#include "structs.h"

Shape *initShapeCube() {
  Triangle *trianglesList = malloc(12 * sizeof(Triangle));
  trianglesList[0] = (Triangle){0, 1, 2};
  trianglesList[1] = (Triangle){0, 2, 3};
  trianglesList[2] = (Triangle){4, 0, 3};
  trianglesList[3] = (Triangle){4, 3, 7};
  trianglesList[4] = (Triangle){5, 4, 7};
  trianglesList[5] = (Triangle){5, 7, 6};
  trianglesList[6] = (Triangle){1, 5, 6};
  trianglesList[7] = (Triangle){1, 6, 2};
  trianglesList[8] = (Triangle){4, 5, 1};
  trianglesList[9] = (Triangle){4, 1, 0};
  trianglesList[10] = (Triangle){2, 6, 7};
  trianglesList[11] = (Triangle){2, 7, 3};

  float3d *vertices = malloc(8 * sizeof(float3d));
  vertices[0] = (float3d){0.5, 0.5, 0.5};
  vertices[1] = (float3d){-0.5, 0.5, 0.5};
  vertices[2] = (float3d){-0.5, -0.5, 0.5};
  vertices[3] = (float3d){0.5, -0.5, 0.5};
  vertices[4] = (float3d){0.5, 0.5, -0.5};
  vertices[5] = (float3d){-0.5, 0.5, -0.5};
  vertices[6] = (float3d){-0.5, -0.5, -0.5};
  vertices[7] = (float3d){0.5, -0.5, -0.5};

  Shape *cube = malloc(sizeof(Shape));

  cube->trianglesList = trianglesList;
  cube->vertices = vertices;
  cube->trianglesCount = 12;
  cube->verticesCount = 8;

  return cube;
}

void freeModel(Shape *shape) {
  free(shape->vertices);
  free(shape->trianglesList);
  free(shape);
}
