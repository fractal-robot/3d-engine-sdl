#ifndef MATRIX_H
#define MATRIX_H

#include "structs.h"

typedef struct {
  float **data;
  int rows;
  int cols;
} Mat;

static inline float getX(const Mat *mat) { return mat->data[0][0]; }
static inline float getY(const Mat *mat) { return mat->data[1][0]; }
static inline float getZ(const Mat *mat) { return mat->data[2][0]; }
static inline float getW(const Mat *mat) { return mat->data[3][0]; }

static inline void setX(const Mat *mat, const float value) {
  mat->data[0][0] = value;
}
static inline void setY(const Mat *mat, const float value) {
  mat->data[1][0] = value;
}
static inline void setZ(const Mat *mat, const float value) {
  mat->data[2][0] = value;
}
static inline void setW(const Mat *mat, const float value) {
  mat->data[3][0] = value;
}

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
float dotProduct(Mat *point, float3d plane);

#endif
