#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"

typedef struct {
  float x;
  float y;
  float z;
} Translate;

typedef struct {
  float x;
  float y;
  float z;
} Scale;

typedef struct {
  float x;
  float y;
  float z;
} Rotate; // right-handed rotation

void translate(Mat *point, const Translate *translate);
Mat *scale(Mat *point, const Scale *scale);
Mat *rotate(Mat *point, const Rotate *rotation);

#endif
