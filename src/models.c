#include "models.h"
#include "definition.h"
#include "structs.h"
#include <stdbool.h>
#include <stdio.h>

Model *createModelCube() {
  Triangle *trianglesList = malloc(12 * sizeof(Triangle));
  trianglesList[0] = (Triangle){0, 1, 2, RED};
  trianglesList[1] = (Triangle){0, 2, 3, RED};
  trianglesList[2] = (Triangle){4, 0, 3, GREEN};
  trianglesList[3] = (Triangle){4, 3, 7, GREEN};
  trianglesList[4] = (Triangle){5, 4, 7, BLUE};
  trianglesList[5] = (Triangle){5, 7, 6, BLUE};
  trianglesList[6] = (Triangle){1, 5, 6, YELLOW};
  trianglesList[7] = (Triangle){1, 6, 2, YELLOW};
  trianglesList[8] = (Triangle){4, 5, 1, PURPLE};
  trianglesList[9] = (Triangle){4, 1, 0, PURPLE};
  trianglesList[10] = (Triangle){2, 6, 7, CYAN};
  trianglesList[11] = (Triangle){2, 7, 3, CYAN};

  float3d *vertices = malloc(8 * sizeof(float3d));
  vertices[0] = (float3d){0.5, 0.5, 0.5};
  vertices[1] = (float3d){-0.5, 0.5, 0.5};
  vertices[2] = (float3d){-0.5, -0.5, 0.5};
  vertices[3] = (float3d){0.5, -0.5, 0.5};
  vertices[4] = (float3d){0.5, 0.5, -0.5};
  vertices[5] = (float3d){-0.5, 0.5, -0.5};
  vertices[6] = (float3d){-0.5, -0.5, -0.5};
  vertices[7] = (float3d){0.5, -0.5, -0.5};

  Model *cube = malloc(sizeof(Model));

  if (vertices == NULL || trianglesList == NULL || cube == NULL) {
    printf("[ERROR] Failed to allocate memory for cube model.\n");
    exit(EXIT_FAILURE);
  }

  cube->trianglesList = trianglesList;
  cube->vertices = vertices;
  cube->trianglesCount = 12;
  cube->verticesCount = 8;

  return cube;
}
