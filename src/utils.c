#include "utils.h"

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