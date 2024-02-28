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

void translate(Mat *point, const Translate *translate);
Mat *scale(Mat *point, const Scale *scale);

#endif
