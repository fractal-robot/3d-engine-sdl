#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
  float x;
  float y;
} Point2d;

typedef struct {
  float x;
  float y;
  float z;
} Point3d;

typedef struct {
  int x;
  int y;
  float hue;
} Point2dCarryHue;

typedef struct {
  char red;
  char green;
  char blue;
} Color;

#endif
