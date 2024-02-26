#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
  float x;
  float y;
} Mat2d;

typedef struct {
  float x;
  float y;
  float z;
} Mat3d;

typedef struct {
  float x;
  float y;
  float hue;
} Mat2dCarryHue;

typedef struct {
  char red;
  char green;
  char blue;
} Color;

typedef struct {
  int a;
  int b;
  int c; // these refers to the index of the coordinates
  Color color;
} Triangle;

#endif
