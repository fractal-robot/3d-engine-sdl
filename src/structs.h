#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
  short red;
  short green;
  short blue;
} Color;

typedef struct {
  int a;
  int b;
  int c; // these refers to the index of the coordinates
  Color color;
} Triangle;

#endif
