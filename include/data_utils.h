#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include "matrix.h"
#include "vector.h"

void loadFeatureMatrix(const char *path, Matrix **X, Vector **y);

#endif