#include "vertex.h"
#include "matrix.h"
#include <stdlib.h>

Vertex *createVertex() {
  Vertex *vertex = malloc(sizeof(Vertex));
  vertex->position = NULL;
  vertex->color = NULL;

  return vertex;
}

void destroyVertex(Vertex *vertex) {
  if (vertex->color != NULL)
    freeMat(vertex->color);
  if (vertex->position != NULL)
    freeMat(vertex->position);
  free(vertex);
}
