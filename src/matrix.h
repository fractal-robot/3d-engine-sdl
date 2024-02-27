#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
  float **data;
  int rows;
  int cols;
} FloatMatrix;

FloatMatrix *createFloatMatrix(int rows, int cols, _Bool initWithZero);
void freeFloatMatrix(const FloatMatrix *matrix);
void setElement(FloatMatrix *mat, int row, int col, float value);
float getElement(FloatMatrix *mat, int row, int col);
FloatMatrix *multiplyMatrices(FloatMatrix *mat1, FloatMatrix *mat2);
void printMatrix(const FloatMatrix *matrix);

#endif
