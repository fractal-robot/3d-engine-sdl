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

#include "matrix.h"
#include "structs.h"
#include <math.h>
#include <stdbool.h>

Mat *createTranslationMatrix(const float3d *values) {
  Mat *translationMat = createMat(4, 4, true);
  translationMat->data[0][0] = 1;
  translationMat->data[1][1] = 1;
  translationMat->data[2][2] = 1;
  translationMat->data[3][3] = 1;

  translationMat->data[0][3] = values->x;
  translationMat->data[1][3] = values->y;
  translationMat->data[2][3] = values->z;

  return translationMat;
}

Mat *createScaleMatrix(const float3d *values) {
  Mat *scalingMat = createMat(4, 4, true);
  scalingMat->data[0][0] = values->x;
  scalingMat->data[1][1] = values->y;
  scalingMat->data[2][2] = values->z;
  scalingMat->data[3][3] = 1;

  return scalingMat;
}

float degToRad(float deg) { return deg * M_PI / 180; }

Mat *createRotationMatrix(const float3d *values) {
  // homogeneous Euler rotation matrix

  float x = degToRad(values->x);
  float y = degToRad(values->y);
  float z = degToRad(values->z);

  Mat *rotationMat = createMat(4, 4, false);
  rotationMat->data[0][0] = cos(y) * cos(z);
  rotationMat->data[0][1] = sin(x) * sin(y) * cos(z) - cos(x) * sin(z);
  rotationMat->data[0][2] = cos(x) * sin(y) * cos(z) + sin(x) * sin(z);
  rotationMat->data[0][3] = 0;
  rotationMat->data[1][0] = cos(y) * sin(z);
  rotationMat->data[1][1] = sin(x) * sin(y) * sin(z) + cos(x) * cos(z);
  rotationMat->data[1][2] = cos(x) * sin(y) * sin(z) - sin(x) * cos(z);
  rotationMat->data[1][3] = 0;
  rotationMat->data[2][0] = -sin(y);
  rotationMat->data[2][1] = sin(x) * cos(y);
  rotationMat->data[2][2] = cos(x) * cos(y);
  rotationMat->data[2][3] = 0;
  rotationMat->data[3][0] = 0;
  rotationMat->data[3][1] = 0;
  rotationMat->data[3][2] = 0;
  rotationMat->data[3][3] = 1;

  return rotationMat;
}
