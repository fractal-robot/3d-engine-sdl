#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
  float **data;
  int rows;
  int cols;
} Mat;

Mat *createMat(int rows, int cols, _Bool initWithZero);
void freeMat(const Mat *matrix);
void setElement(Mat *mat, int row, int col, float value);
float getElement(Mat *mat, int row, int col);
Mat *multiplyMat(Mat *mat1, Mat *mat2);
void printMat(const Mat *matrix);
void normalizeMat(Mat *mat);
Mat *crossMat(Mat *u, Mat *v);
Mat *substractMat(Mat *u, Mat *v);
void assignArray(Mat *mat, float *values);
#endif
