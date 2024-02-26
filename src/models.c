
#include "models.h"
#include "definition.h"
#include "helper.h"
#include "structs.h"

void renderModel(SDL_Renderer *renderer, Model *model) {
  Stack *projected = createStack(model->verticesCount);
  for (int v = 0; v < model->verticesCount; ++v) {
    Mat3d translated = translate(model->vertices[v]);
    Mat2d *vertex = malloc(sizeof(Mat2d));
    *vertex = projectVertex(&translated);
    printf("Calculated projection for %f %f\n", vertex->x, vertex->y);
    push(projected, vertex);
  }

  for (int t = 0; t < model->trianglesCount; ++t) {
    Triangle triangle = model->trianglesList[t];
    Mat2d v1 = *(Mat2d *)getStackItem(projected, triangle.a);
    Mat2d v2 = *(Mat2d *)getStackItem(projected, triangle.b);
    Mat2d v3 = *(Mat2d *)getStackItem(projected, triangle.c);
    printf("Point 1: %f %f\n", v1.x, v1.y);
    printf("Point 2: %f %f\n", v2.x, v2.y);
    printf("Point 3: %f %f\n", v3.x, v3.y);
    printf("Accesing vertices %i %i %i\n", triangle.a, triangle.b, triangle.c);
    drawWireframeTriangle(renderer, v1, v2, v3, &triangle.color);
  }
}

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

  Mat3d *vertices = malloc(8 * sizeof(Mat3d));
  vertices[0] = (Mat3d){1, 1, 1};
  vertices[1] = (Mat3d){-1, 1, 1};
  vertices[2] = (Mat3d){-1, -1, 1};
  vertices[3] = (Mat3d){1, -1, 1};
  vertices[4] = (Mat3d){1, 1, -1};
  vertices[5] = (Mat3d){-1, 1, -1};
  vertices[6] = (Mat3d){-1, -1, -1};
  vertices[7] = (Mat3d){1, -1, -1};

  for (int v = 0; v < 8; ++v) {
    Mat2d val = projectVertex(&vertices[v]);
    printf("at index %i, got %f %f\n", v, val.x, val.y);
  }

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
