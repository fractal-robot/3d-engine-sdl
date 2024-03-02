#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"
#include "structs.h"

Mat *createTranslationMatrix(const float3d *values);
Mat *createScaleMatrix(const float3d *values);
Mat *createRotationMatrix(const float3d *values);

#endif
