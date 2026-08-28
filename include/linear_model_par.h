#ifndef LINEAR_MODEL_PAR_H
#define LINEAR_MODEL_PAR_H

#include "matrix_par.h"
#include "vector_par.h"
#include "linear_model.h"

void linear_model_par_learn(LinearModel *model, Matrix *X_train, Vector *y_train);
Vector* linear_model_par_predict(LinearModel *model, Matrix *X_test);

#endif