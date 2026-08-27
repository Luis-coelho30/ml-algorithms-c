#include <math.h>
#include <stdio.h>

typedef struct {
    double* data;
    int size;
} Vector;

/*
    Creates a vector with the provided argument data
    
    Arguments:
        double* data - the component data of the initialized vector. Defaults to 0 if NULL 
        int size - the size of the desired vector

    returns 
        A pointer to a vector type struct
*/
Vector* create_vector(const double* data, int size) {
    
    if(size >= 1) {

        Vector* v = malloc(sizeof(Vector));

        if(v != NULL) {
            v->size = size;

            if(data != NULL) {
                v->data = malloc(size * sizeof(double));
                for(int i = 0; i < size; i++) {
                    v->data[i] = data[i];
                }
            }

            else {
                v->data = calloc(v->size, sizeof(double));
            }

            return v;
        }
    }

    return NULL;
}

/*
    Frees the vector's data and zeroes it's size. Must be called to avoid Memory Leaks 
    
    Arguments:
        Vector* v - The vector to be freed
*/
void free_vector(Vector* v) {
    free(v->data);
    free(v);
}

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
double dot_product(const Vector* vec1, const Vector* vec2) {
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

void cross_product(const Vector* vec1, const Vector* vec2) {

}

void triple_product(const Vector* vec1, const Vector* vec2, const Vector* vec3) {

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
int multiply_scalar(Vector* vec1, double scalar) {
    if(vec1 == NULL)
        return -1;

    for(int i = 0; i < vec1->size; i++) {
        vec1->data[i] *= scalar;
    }

    return 0;
}
