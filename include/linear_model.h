#ifndef LINEAR_MODEL_H
#define LINEAR_MODEL_H

#include "vector.h"
#include "matrix.h"

typedef struct {
    Vector *weights;
    double bias;
    double learning_rate;
    int epochs;
} LinearModel;

LinearModel *linear_model_init(int n_features, double lr, int epochs);
void linear_model_learn(LinearModel *model, Matrix *X_train, Vector *y_train);
Vector* linear_model_predict(LinearModel *model, Matrix *X_test);
void linear_model_free(LinearModel *model);

#endif