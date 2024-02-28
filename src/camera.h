#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"

typedef struct {
  Mat *pos;
  Mat *target;
  Mat *direction;
  Mat *up;
  Mat *right;
  Mat *lookAt;
} Camera; // position, target and up vector have to be specified

void setCameraProp(Camera *c);

#endif
