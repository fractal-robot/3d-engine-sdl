// Code to generate the different transformations as follow:
// 1. scale
// 2. rotate
// 3. translate
// 4. camera transform (translate then rotate)
// 5. perspective equation
// 6. viewport to canva

// The camera transformations are done by applying the invert to all instances
// of the scene.

// We need to recalculate the:
// - projection matrix only when window size / FOV is changed.
// - camera matrix every frame.
// - model matrix when the object move, constant for object that don't move.

// So we have to implement a signal system. Each transformation will hold it's
// actual state, an will be reloadable if needed.

// Each instance will contains its own rendering pipeline.

// If a transformation have been recalculated, then we need to compute again the
// whole rendering pipeline from the first new transformation.

// https://wikimedia.org/api/rest_v1/media/math/render/svg/a6821937d5031de282a190f75312353c970aa2df

#include "transform.h"
#include "matrix.h"
#include <stdbool.h>

Matrix *translate(Matrix *point, const Translate *translate) {
  Matrix *translationMatrix = createMatrix(1, 4, false);
  translationMatrix->data[0][1] = translate->x;
  translationMatrix->data[0][2] = translate->y;
  translationMatrix->data[0][3] = translate->z;
  translationMatrix->data[0][4] = 1;
  return multiplyMatrices(translationMatrix, point);
}

Matrix *scale(Matrix *point, const Scale *scale) {
  Matrix *scalarMatrix = createMatrix(4, 4, true);
  scalarMatrix->data[0][0] = scale->x;
  scalarMatrix->data[1][1] = scale->y;
  scalarMatrix->data[2][2] = scale->z;
  scalarMatrix->data[3][3] = 1;
  return multiplyMatrices(scalarMatrix, point);
}
