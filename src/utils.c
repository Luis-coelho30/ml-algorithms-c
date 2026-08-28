#include <math.h>

#include "utils.h"
#include "vector.h"
#include "matrix.h"

static void swap_double(double *a, double *b)
{
    double tmp = *a;
    *a = *b;
    *b = tmp;
}

static void swap_int(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static int partition(double *distances, int *indices, int left, int right)
{
    double pivot = distances[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (distances[j] <= pivot) {
            i++;
            swap_double(&distances[i], &distances[j]);
            swap_int(&indices[i], &indices[j]);
        }
    }

    swap_double(&distances[i + 1], &distances[right]);
    swap_int(&indices[i + 1], &indices[right]);

    return i + 1;
}

/*
    Partitions the distances array around the k-th smallest element,
    placing its corresponding index in the same position.

    Arguments:
        double* distances
        int* indices
        int left
        int right
        int k

*/
void quickselect(double *distances, int *indices, int left, int right, int k)
{
    if (left >= right)
        return;

    int pivot_idx = partition(distances, indices, left, right);

    if (pivot_idx == k)
        return;
    else if (k < pivot_idx)
        quickselect(distances, indices, left, pivot_idx - 1, k);
    else
        quickselect(distances, indices, pivot_idx + 1, right, k);
}

/*
    Computes the accuracy of a classification model

    Arguments:
        predictions - the predicted class labels
        y_test - the ground truth class labels
    Returns:
        correct predictions / total predictions as a double between 0 and 1
*/
double accuracy(const Vector* predictions, const Vector* y_test) {
    int correct = 0;
    for (int i = 0; i < predictions->size; i++)
        if ((int)predictions->data[i] == (int)y_test->data[i])
            correct++;
    return (double)correct / predictions->size;
}

/*
    Computes the confusion matrix for a classification model

    Arguments:
        predictions - the predicted class labels
        y_test - the ground truth class labels
        n_classes - the number of distinct classes
    Returns:
        NULL on failure to allocate the matrix
        A (n_classes x n_classes) Matrix* where row = actual, col = predicted
*/
Matrix* confusion_matrix(const Vector* predictions, const Vector* y_test, int n_classes) {
    Matrix *cm = create_matrix(NULL, n_classes, n_classes);
    for (int i = 0; i < predictions->size; i++) {
        int actual = (int)y_test->data[i];
        int predicted = (int)predictions->data[i];
        matrix_set(cm, actual, predicted, matrix_get(cm, actual, predicted) + 1.0);
    }
    return cm;
}

/*
    Computes the Root Mean Squared Error of a regression model

    Arguments:
        predictions - the predicted continuous values
        y_test - the ground truth continuous values
    Returns:
        RMSE as a double in the same units as the target
*/
double rmse(Vector* predictions, Vector* y_test) {
    double sum = 0.0;
    for (int i = 0; i < predictions->size; i++) {
        double diff = predictions->data[i] - y_test->data[i];
        sum += diff * diff;
    }
    return sqrt(sum / predictions->size);
}