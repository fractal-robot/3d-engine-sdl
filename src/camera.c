#include "camera.h"
#include "matrix.h"
#include <stdbool.h>
#include <stdlib.h>

void setCameraProp(Camera *camera) {
  /*

  c->direction = substractMat(c->pos, c->target);
  normalizeMat(c->direction);

  // Gram-Schmidt process
  {
    Mat *up = createMat(3, 1, false);
    up->data[0][0] = 0;
    up->data[1][0] = 1;
    up->data[2][0] = 0;
    c->right = crossMat(up, c->direction);
    freeMat(up);
  }
  c->up = crossMat(c->direction, c->right);

  */

  // initialize the look at matrix
  // for now we only care about space translation

  float posArr[4][4] = {
      {1, 0, 0, -getX(camera->pos)},
      {0, 1, 0, -getY(camera->pos)},
      {0, 0, 1, -getZ(camera->pos)},
      {0, 0, 0, 1},
  };

  float *posArrPtr = *posArr;
  assignArray(camera->lookAt, posArrPtr);
}

void initCamera(Camera *camera) {
  camera->pos = NULL;
  camera->target = NULL;
  camera->direction = NULL;
  camera->up = NULL;
  camera->right = NULL;
  camera->lookAt = createMat(4, 4, false);
}

void destroyCamera(Camera *camera) {
  freeMat(camera->pos);
  freeMat(camera->target);
  freeMat(camera->direction);
  freeMat(camera->up);
  freeMat(camera->right);
  freeMat(camera->lookAt);
  free(camera);
}
