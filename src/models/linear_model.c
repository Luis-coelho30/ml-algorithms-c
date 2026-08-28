#include "vector.h"
#include "matrix.h"
#include "linear_model.h"
#include <stdlib.h>

LinearModel* linear_model_init(int n_features, double lr, int epochs);
void linear_model_learn(LinearModel* model, Matrix* X_train, Vector* y_train);
Vector* linear_model_predict(LinearModel* model, Matrix *X_test);
void linear_model_free(LinearModel* model);

/*
    Creates a linear model given the number of features, learning rate and number of epochs

    Arguments:
        n_features - The number of features the model will be trained on
        lr - The learning rate of the model
        epochs - The ammount of times the model will be trained on the dataset
    
    Returns:
        NULL if any parameters are lesser than 1
        NULL on failure to allocate the model or weight vector
        The model on success 
*/
LinearModel* linear_model_init(int n_features, double lr, int epochs) {
    if(n_features > 0 && lr > 0 && epochs > 0) {

        LinearModel* model = malloc(sizeof(LinearModel));

        if(model!=NULL) {
            
            model->weights = create_vector(NULL, n_features);
            
            if(model->weights!=NULL) {
                model->learning_rate = lr;
                model->epochs = epochs;
                model->bias = 0.0;

                return model;
            }
            else {
                free(model);
            }
        }

    }

    return NULL;
}

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
void linear_model_learn(LinearModel* model, Matrix* X_train, Vector* y_train) {

    if(model!=NULL) {

        int epochs = model->epochs;

        Matrix* X_t = matrix_transpose(X_train);
        for(int i = 0; i < epochs; i++) {

            // predictions = matrix_vec_multiply(X, weights)
            Vector* predictions = matrix_vec_multiply(X_train, model->weights);

            // error[i] = predictions[i] - y[i]
            vector_sub_in_place(predictions, y_train);

            // gradient = matrix_vec_multiply(X^T, error)
            Vector* gradient = matrix_vec_multiply(X_t, predictions);
            
            //scale gradient by 1/(number of samples)
            multiply_scalar(gradient, 1.0 / X_train->rows);

            // bias_grad = sum(error) / m
            double bias_grad = vector_component_sum(predictions) / X_train->rows;
            // bias -= lr * bias_grad
            model->bias -= model->learning_rate * bias_grad;

            // lr * gradient[j]
            multiply_scalar(gradient, model->learning_rate);
            // weights[j] -= lr * gradient[j]
            vector_sub_in_place(model->weights, gradient);

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
Vector* linear_model_predict(LinearModel* model, Matrix *X_test) {

    if(model!=NULL && X_test!=NULL) {
        Vector* predictions = matrix_vec_multiply(X_test, model->weights);
        if(predictions!=NULL) {
            sum_scalar(predictions, model->bias);

            return predictions;
        }
    }
    
    return NULL;
}

/*
    Frees the memory allocated by the model 

    Arguments:
        LinearModel* model - The model that will be destroyed

*/
void linear_model_free(LinearModel* model) {
    free_vector(model->weights);
    free(model);
}