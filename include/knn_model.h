#ifndef KNN_H
#define KNN_H

#include "vector.h"
#include "matrix.h"

typedef struct {
    Matrix *X_train;
    Vector *y_train;
    int k;
} KNNModel;

KNNModel *knn_model_init(int n_neighbours);
void knn_model_learn(KNNModel *model, Matrix *X_train, Vector *y_train);
Vector *knn_model_classify(KNNModel *model, Matrix *X_test);
void knn_model_free(KNNModel *model);

#endif