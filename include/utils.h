#ifndef UTILS_H
#define UTILS_H

#include "vector.h"
#include "matrix.h"

void quickselect(double *distances, int *indices, int left, int right, int k);

// ML accuracy

double accuracy(const Vector *predictions, const Vector *y_test);
Matrix* confusion_matrix(const Vector* predictions, const Vector* y_test, int n_classes);
double rmse(Vector *predictions, Vector *y_test);

#endif
