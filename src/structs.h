#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
  short red;
  short green;
  short blue;
} Color;

typedef struct {
  float x;
  float y;
  float z;
} float3d;

typedef struct {
  int a;
  int b;
  int c; // these refers to the index of the coordinates
} Triangle;

#endif
