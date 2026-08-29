#include <stdlib.h>
#include <stdio.h>
#include "knn_model_par.h"

Vector* knn_model_par_classify(KNNModel* model, Matrix *X_test);

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
Vector* knn_model_par_classify(KNNModel* model, Matrix *X_test) {

    if(model!=NULL && X_test!=NULL) {
        int alloc_failed = 0;
        int n_test = X_test->rows;
        int n_train = model->X_train->rows;

        Vector* predictions = create_vector(NULL, n_test);
            
        if(predictions==NULL) {
            return NULL;
        }

        #pragma omp parallel for
        for(int i = 0; i < n_test; i++) {

            double* distances = malloc(n_train * sizeof(double));
            int* indexes = malloc(n_train * sizeof(int));

            if(distances == NULL || indexes == NULL) {
                free(distances);
                free(indexes);
                #pragma omp atomic write
                alloc_failed = 1;
            }

            if(!alloc_failed) {
                
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

                free(distances);
                free(indexes);
            }
        }

        if(alloc_failed) {
            free_vector(predictions);
            return NULL;
        }

        return predictions;
    }

    return NULL;
}