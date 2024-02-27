#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
  float **data;
  int rows;
  int cols;
} Matrix;

Matrix *createMatrix(int rows, int cols, _Bool initWithZero);
void freeMatrix(const Matrix *matrix);
void setElement(Matrix *mat, int row, int col, float value);
float getElement(Matrix *mat, int row, int col);
Matrix *multiplyMatrices(Matrix *mat1, Matrix *mat2);
void printMatrix(const Matrix *matrix);

#endif
