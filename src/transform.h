#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"

typedef struct {
  float x;
  float y;
  float z;
} float3d;

void translate(Mat *point, const float3d *translate);
Mat *scale(Mat *point, const float3d *scale);
Mat *rotate(Mat *point, const float3d *rotation);

#endif
