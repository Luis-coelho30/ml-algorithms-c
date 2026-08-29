#include <stdio.h>
#include <time.h>
#include "data_utils.h"
#include "utils.h"
#include "matrix.h"
#include "vector.h"
#include "linear_model.h"
#include "knn_model.h"

void test_linear_regression() {
    printf("=== Linear Regression Test ===\n");

    Matrix *X_train;
    Vector *y_train;
    //loadFeatureMatrix("data/train_lr.csv", &X_train, &y_train);
    loadFeatureMatrix("data/housing_lr_train.csv", &X_train, &y_train);

    LinearModel *model = linear_model_init(X_train->cols, 0.0001, 10000);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    linear_model_learn(model, X_train, y_train);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double learn_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    Matrix *X_test;
    Vector *y_test;
    loadFeatureMatrix("data/housing_lr_test.csv", &X_test, &y_test);

    clock_gettime(CLOCK_MONOTONIC, &start);
    Vector *predictions = linear_model_predict(model, X_test);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double predict_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // for (int i = 0; i < predictions->size; i++)
    //     printf("Sample %d: predicted=%.2f expected=%.2f\n", i, predictions->data[i], y_test->data[i]);

    printf("RMSE: %.4f\n", rmse(predictions, y_test));
    printf("Learn time:   %.4fs\n", learn_time);
    printf("Predict time: %.4fs\n", predict_time);

    free_vector(predictions);
    free_vector(y_test);
    free_vector(y_train);
    free_matrix(X_train);
    free_matrix(X_test);
    linear_model_free(model);
}

void test_knn() {
    printf("=== KNN Classification Test ===\n");

    Matrix *X_train;
    Vector *y_train;
    //loadFeatureMatrix("data/train_knn.csv", &X_train, &y_train);
    loadFeatureMatrix("data/housing_knn_train.csv", &X_train, &y_train);


    KNNModel *model = knn_model_init(5);
    knn_model_learn(model, X_train, y_train);

    Matrix *X_test;
    Vector *y_test;
    loadFeatureMatrix("data/housing_knn_test.csv", &X_test, &y_test);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    Vector *predictions = knn_model_classify(model, X_test);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double classify_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // for (int i = 0; i < predictions->size; i++)
    //     printf("Sample %d: predicted=%d expected=%d\n", i, (int)predictions->data[i], (int)y_test->data[i]);

    printf("Accuracy: %.4f\n", accuracy(predictions, y_test));
    printf("Classify time: %.4fs\n", classify_time);

    Matrix *cm = confusion_matrix(predictions, y_test, 5);
    matrix_print(cm);
    free_matrix(cm);

    free_vector(predictions);
    free_vector(y_test);
    free_vector(y_train);
    free_matrix(X_train);
    free_matrix(X_test);
    knn_model_free(model);
}

int main() {
    test_linear_regression();
    test_knn();
    return 0;
}