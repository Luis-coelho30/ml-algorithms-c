#ifndef KNN_PAR_H
#define KNN_PAR_H

#include "knn_model.h"
#include "utils.h"
#include "matrix_par.h"
#include "vector_par.h"

Vector *knn_model_par_classify(KNNModel *model, Matrix *X_test);

#endif