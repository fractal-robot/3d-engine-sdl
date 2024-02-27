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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Matrix *translate(Matrix *point, Matrix *translationMatrix) {
  if (translationMatrix->rows != 1 || translationMatrix->cols != 4 ||
      translationMatrix->data[1][4] != 0) {
    printf("[ERROR] Incompatible translation matrix for translation.");
    exit(EXIT_FAILURE);
  }
  if (point->rows != 1 || point->cols != 4 || point->data[1][4] != 1) {
    printf("[ERROR] Incompatible point for translation.");
    exit(EXIT_FAILURE);
  }

  return multiplyMatrices(translationMatrix, point);
}

Matrix *scale(Matrix *point, Matrix *scalarMatrix) {
  if (scalarMatrix->rows != 4 || scalarMatrix->cols != 4) {
    printf("[ERROR] Wrong dimension of scalar matrix for scaling.");
    exit(EXIT_FAILURE);
  }
}
