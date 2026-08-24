#include <stdio.h>
#include <stdlib.h>
#include "data_utils.h"

void computePrediction(int i, int j);
void computeError(int h, int y);
void computeGradient();
void updateWeights();
void freeFeatureMatrix();

int main() {
    double **feature_matrix;
    double *target_vector;

    int rows, cols;

    loadFeatureMatrix("data/housing.csv", &feature_matrix, &target_vector, &rows, &cols);

    // Initialize parameter_matrix and bias to 0
    double parameter_vector[cols];

    for(int i = 0; i < cols; i++) {
        parameter_vector[i] = 0;
    }

    double b = 0; 

    // Compute Predictions

    // Compute Error

    // Compute Gradients

    // Update Error

    // Free Memory

    return 0;
}