#include "vector.h"
#include "matrix.h"
#include "utils.h"
#include "knn_model.h"
#include <stdlib.h>
#include <stdio.h>

KNNModel* knn_model_init(int n_neighbours);
void knn_model_learn(KNNModel* model, Matrix* X_train, Vector* y_train);
Vector* knn_model_classify(KNNModel* model, Matrix *X_test);
void knn_model_free(KNNModel* model);

/*
    Creates a KNN model given the number of neighbours

    Arguments:
        n_neighbours - amount of neighbour data used to classify a sample
    
    Returns:
        NULL if n_neighbours is lesser than 1
        NULL on failure to allocate the model
        The model on success 
*/
KNNModel* knn_model_init(int n_neighbours) {
    if(n_neighbours > 0) {

        KNNModel* model = malloc(sizeof(KNNModel));

        if(model!=NULL) {
            model->k = n_neighbours;
            model->X_train = NULL;
            model->y_train = NULL;

            return model;
        }
    }

    return NULL;
}

/*
    Sets the feature matrix dataset and the class boundary on the model

    Arguments:
        KNNModel* model - The model that will receive the datasets
        Matrix* X_train - The Training Dataset
        Vector* y_train - The class boundary dataset
    
    Returns:
        NULL if any parameters are NULL
        The model on success 
*/
void knn_model_learn(KNNModel* model, Matrix* X_train, Vector* y_train) {

    if(model!=NULL && X_train!=NULL && y_train!=NULL) {
        model->X_train = X_train;
        model->y_train = y_train;
    }
}

/*
    Classifies a series of samples given a dataset of n_features 

    Arguments:
        KNNModel* model - The model that will make the predictions 
        Matrix* X_test - The dataset of n_features
    
    Returns:
        NULL if either the model or the dataset are NULL
        NULL on failure to allocate the predictions vector
        A vector containing the predictions
*/
Vector* knn_model_classify(KNNModel* model, Matrix *X_test) {

    if(model!=NULL && X_test!=NULL) {
        int n_test = X_test->rows;
        int n_train = model->X_train->rows;

        Vector* predictions = create_vector(NULL, n_test);
        double* distances = malloc(n_train * sizeof(double));
        int* indexes = malloc(n_train * sizeof(int));
            
        if(predictions==NULL) {
            return NULL;
        }
        if(distances==NULL) {
            free_vector(predictions);
            return NULL;
        }
        if(indexes==NULL) {
            free_vector(predictions);
            free(distances);
            return NULL;
        }


        for(int i = 0; i < n_test; i++) {
            double* point = matrix_get_row(X_test, i);
            for(int j = 0; j < n_train; j++) {
                indexes[j] = j;
            }
            row_distance(model->X_train, point, distances);
            quickselect(distances, indexes, 0, n_train - 1, model->k);
            int class_counts[5] = {0};
            for (int t = 0; t < model->k; t++)
                class_counts[(int)vector_get(model->y_train, indexes[t])]++;
                
            int majority = 0;
            for (int c = 1; c < 5; c++)
                if (class_counts[c] > class_counts[majority])
                    majority = c;
            vector_set(predictions, i, (double)majority); 
        }

        free(distances);
        free(indexes);
        return predictions;
    }

    return NULL;
}

/*
    Frees the memory allocated by the model 

    Arguments:
        KNNModel* model - The model that will be destroyed

*/
void knn_model_free(KNNModel* model) {
    free(model);
}