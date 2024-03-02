#ifndef VERTEX_H
#define VERTEX_H

#include "matrix.h"

typedef struct {
  Mat *position;
  Mat *color;
} Vertex;

Vertex *createVertex();
void destroyVertex(Vertex *vertex);

#endif
