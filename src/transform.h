#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"
#include "structs.h"

void translate(Mat *point, const float3d *translate);
Mat *scale(Mat *point, const float3d *scale);
Mat *rotate(Mat *point, const float3d *rotation);

#endif
