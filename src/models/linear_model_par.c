#include "linear_model_par.h"
#include <stdlib.h>

/*
    Runs the Linear Regression Algorithm on the model

    Arguments:
        LinearModel* model - The model that will be trained 
        Matrix* X_train - The Training Dataset
        Vector* y_train - The Target values
    
    Returns:
        NULL if any parameters are lesser than 1
        NULL on failure to allocate the model or weight vector
        The model on success 
*/
void linear_model_par_learn(LinearModel* model, Matrix* X_train, Vector* y_train) {

    if(model!=NULL) {

        int epochs = model->epochs;

        Matrix* X_t = matrix_transpose_par(X_train);
        for(int i = 0; i < epochs; i++) {

            // predictions = matrix_vec_multiply(X, weights)
            Vector* predictions = matrix_vec_multiply_par(X_train, model->weights);

            // error[i] = predictions[i] - y[i]
            vector_sub_in_place_par(predictions, y_train);

            // gradient = matrix_vec_multiply(X^T, error)
            Vector* gradient = matrix_vec_multiply_par(X_t, predictions);
            
            //scale gradient by 1/(number of samples)
            multiply_scalar_par(gradient, 1.0 / X_train->rows);

            // bias_grad = sum(error) / m
            double bias_grad = vector_component_sum_par(predictions) / X_train->rows;
            // bias -= lr * bias_grad
            model->bias -= model->learning_rate * bias_grad;

            // lr * gradient[j]
            multiply_scalar_par(gradient, model->learning_rate);
            // weights[j] -= lr * gradient[j]
            vector_sub_in_place_par(model->weights, gradient);

            //free vectors
            free_vector(predictions);
            free_vector(gradient);
        }
        free_matrix(X_t);
    }
}

/*
    Predicts a series of outputs given a dataset of n_features 

    Arguments:
        LinearModel* model - The model that will make the predictions 
        Matrix* X_test - The dataset of n_features
    
    Returns:
        NULL if either the model or the dataset are NULL
        NULL on failure to allocate the predictions vector
        A vector containing the predictions
*/
Vector* linear_model_par_predict(LinearModel* model, Matrix *X_test) {

    if(model!=NULL && X_test!=NULL) {
        Vector* predictions = matrix_vec_multiply_par(X_test, model->weights);
        if(predictions!=NULL) {
            sum_scalar_par(predictions, model->bias);

            return predictions;
        }
    }
    
    return NULL;
}