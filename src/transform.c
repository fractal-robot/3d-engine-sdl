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

void translate(Mat *point, const Translate *translate) {
  point->data[0][0] += translate->x;
  point->data[1][0] += translate->y;
  point->data[2][0] += translate->z;
}

Mat *scale(Mat *point, const Scale *scale) {
  Mat *scalarMat = createMat(4, 4, true);
  scalarMat->data[0][0] = scale->x;
  scalarMat->data[1][1] = scale->y;
  scalarMat->data[2][2] = scale->z;
  scalarMat->data[3][3] = 1;
  Mat *newPoint = multiplyMat(scalarMat, point);
  freeMat(point);
  return newPoint;
}
