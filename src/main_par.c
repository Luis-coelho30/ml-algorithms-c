#include <stdio.h>
#include <omp.h>
#include "data_utils.h"
#include "utils.h"

#include "linear_model_par.h"

void test_linear_regression_par(int n_threads) {
    printf("=== Linear Regression Parallel Test (threads=%d) ===\n", n_threads);

    omp_set_num_threads(n_threads);

    Matrix *X_train;
    Vector *y_train;
    loadFeatureMatrix("data/housing_lr_train.csv", &X_train, &y_train);

    LinearModel *model = linear_model_init(X_train->cols, 0.0001, 10000);

    double start = omp_get_wtime();
    linear_model_par_learn(model, X_train, y_train);
    double learn_time = omp_get_wtime() - start;

    Matrix *X_test;
    Vector *y_test;
    loadFeatureMatrix("data/housing_lr_test.csv", &X_test, &y_test);

    start = omp_get_wtime();
    Vector *predictions = linear_model_par_predict(model, X_test);
    double predict_time = omp_get_wtime() - start;

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

int main() {
    int thread_counts[] = {1, 2, 4, 8};
    for (int t = 0; t < 4; t++)
        test_linear_regression_par(thread_counts[t]);
    return 0;
}