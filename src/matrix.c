#include "matrix.h"
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

void assignArray(Mat *mat, float **values) {
  for (int i = 0; i < mat->rows; ++i)
    for (int j = 0; j < mat->cols; ++j)
      mat->data[i][j] = values[i][j];
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
