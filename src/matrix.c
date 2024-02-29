#include "matrix.h"
#include "structs.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Mat *createMat(int rows, int cols, _Bool initWithZero) {
  Mat *mat = malloc(sizeof(Mat));
  if (mat == NULL) {
    printf("[ERROR] Failed to allocate matrix.\n");
    exit(EXIT_FAILURE);
  }

  mat->rows = rows;
  mat->cols = cols;

  mat->data = (float **)malloc(rows * sizeof(float *));
  if (mat->data == NULL) {
    printf("[ERROR] Failed to allocate matrix.\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < rows; ++i) {
    mat->data[i] = (float *)malloc(cols * sizeof(float));
    if (mat->data[i] == NULL) {
      printf("[ERROR] Failed to allocate matrix.\n");
      exit(EXIT_FAILURE);
    }
    if (initWithZero)
      for (int j = 0; j < cols; ++j)
        mat->data[i][j] = 0.0;
  }

  return mat;
}

void assignArray(Mat *mat, float *values) {
  for (int i = 0; i < mat->rows; ++i)
    for (int j = 0; j < mat->cols; ++j)
      mat->data[i][j] = values[i * mat->cols + j];
}

void freeMat(const Mat *matrix) {
  for (int i = 0; i < matrix->rows; ++i)
    free(matrix->data[i]);
  free(matrix->data);
  free((void *)matrix);
}

void setElement(Mat *mat, int row, int col, float value) {
  if (row < 0 || row >= mat->rows || col < 0 || col >= mat->cols) {
    printf("[ERROR] Failed to set element to matrix: index out of bounds.\n");
    exit(EXIT_FAILURE);
  }

  mat->data[row][col] = value;
}

float getElement(Mat *mat, int row, int col) {
  if (row < 0 || row >= mat->rows || col < 0 || col >= mat->cols) {
    printf("[ERROR] Failed to get element from matrix: index out of bounds.\n");
    exit(EXIT_FAILURE);
  }

  return mat->data[row][col];
}

Mat *multiplyMat(Mat *mat1, Mat *mat2) {
  if (mat1->cols != mat2->rows) {
    printf("[ERROR] Failed to multiply matrices: incompatible dimensions.\n");
    printf("First matrix %i %i \n", mat1->rows, mat1->cols);
    printf("Second matrix %i %i \n", mat2->rows, mat2->cols);
    exit(EXIT_FAILURE);
  }

  Mat *result = createMat(mat1->rows, mat2->cols, 1);

  for (int i = 0; i < mat1->rows; ++i)
    for (int j = 0; j < mat2->cols; ++j)
      for (int k = 0; k < mat1->cols; ++k)
        result->data[i][j] += mat1->data[i][k] * mat2->data[k][j];

  return result;
}

void printMat(const Mat *matrix) {
  printf("\n\n---\n");

  for (int i = 0; i < matrix->rows; ++i) {
    for (int j = 0; j < matrix->cols; ++j)
      printf("%10f", matrix->data[i][j]);
    printf("\n");
  }

  printf("\n---\n\n");
}

void normalizeMat(Mat *mat) {
  assert(mat->cols == 1);

  float length = 0.0;
  for (int i = 0; i < mat->rows; ++i)
    length += mat->data[i][0] * mat->data[i][0];
  length = sqrt(length);

  for (int i = 0; i < mat->rows; ++i)
    mat->data[i][0] = mat->data[i][0] / length;
}

Mat *crossMat(Mat *u, Mat *v) {
  assert(u->rows == 3 && v->rows == 3 && u->cols == 1 && v->cols == 1);

  Mat *product = createMat(3, 1, false);

  product->data[0][0] =
      u->data[1][0] * v->data[2][0] - u->data[2][0] * v->data[1][0];
  product->data[1][0] =
      u->data[2][0] * v->data[0][0] - u->data[0][0] * v->data[2][0];
  product->data[2][0] =
      u->data[0][0] * v->data[1][0] - u->data[1][0] * v->data[0][0];

  return product;
}

float dotProduct(Mat *point, float3d plane) {
  assert(point->rows == 4 && point->cols == 1);
  return point->data[0][0] * plane.x + point->data[1][0] * plane.y +
         point->data[2][0] * plane.y;
}

Mat *substractMat(Mat *u, Mat *v) {
  assert(u->rows == v->rows && u->cols == 1 && v->cols == 1);

  Mat *substracted = createMat(u->rows, 1, false);

  for (int i = 0; i < u->rows; ++i)
    substracted->data[i][0] = u->data[i][0] - v->data[i][0];

  return substracted;
}
