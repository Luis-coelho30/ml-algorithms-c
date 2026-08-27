#include <math.h>
#include <stdio.h>

typedef struct {
    double* data;
    int size;
} Vector;

/*
    Calculates the dot product of two vectors
    
    Arguments:
        Vector vec1 - first operand 
        Vector vec2 - second operand

    returns 
        NaN if either vector is NULL | 
        NaN if the vectors have different sizes | 
        The value of the dot_product on success
*/
double dot_product(Vector* vec1, Vector* vec2) {
    if(vec1 == NULL || vec2 == NULL)
        return NAN;
    if(vec1->size != vec2->size)
        return NAN;

    double result = 0;

    for(int i = 0; i < vec1->size; i++) {
        result += vec1->data[i] * vec2->data[i];
    }

    return result;
}

void cross_product(Vector* vec1, Vector* vec2) {

}

void triple_product(Vector* vec1, Vector* vec2, Vector* vec3) {

}


/*
    Modifies vec1 in-place by taking the element-wise sum of both vectors
    
    Arguments:
        Vector vec1 - destination vector and first operand 
        Vector vec2 - second operand

    returns 
        -1 if either vector is NULL | 
        -1 if the vectors have different sizes | 
        0 on success
*/
int sum_vector(Vector* vec1, Vector* vec2) {
    if(vec1 == NULL || vec2 == NULL)
        return -1;
    if(vec1->size != vec2->size)
        return -1;

    for(int i = 0; i < vec1->size; i++) {
        vec1->data[i] += vec2->data[i];
    }

    return 0;
}

/*
    Modifies vec1 in-place by taking the element-wise sum of a vector by a scalar
    
    Arguments:
        Vector vec1 - destination vector and first operand 
        double scalar - second operand

    returns 
        -1 if the vector is NULL |  
        0 on success
*/
int sum_scalar(Vector* vec1, double scalar) {
    if(vec1 == NULL)
        return -1;

    for(int i = 0; i < vec1->size; i++) {
        vec1->data[i] += scalar;
    }

    return 0;
}

/*
    Modifies vec1 in-place by taking the element-wise multiplication of a vector by a scalar
    
    Arguments:
        Vector vec1 - destination vector and first operand 
        double scalar - second operand

    returns 
        -1 if the vector is NULL |  
        0 on success
*/
void multiply_scalar(Vector* vec1, double scalar) {
    if(vec1 == NULL)
        return -1;

    for(int i = 0; i < vec1->size; i++) {
        vec1->data[i] *= scalar;
    }

    return 0;
}
