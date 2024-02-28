#include "camera.h"
#include "matrix.h"
#include <stdbool.h>

void setCameraProp(Camera *c) {
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
  c->lookAt = createMat(4, 4, false);

  float posArr[4][4] = {
      {1, 0, 0, -c->pos->data[0][0]},
      {0, 1, 0, -c->pos->data[1][0]},
      {0, 0, 1, -c->pos->data[2][0]},
      {0, 0, 0, 1},
  };

  float *posArrPtr = *posArr;

  assignArray(c->lookAt, posArrPtr);
}
