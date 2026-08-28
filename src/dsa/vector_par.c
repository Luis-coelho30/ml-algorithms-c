#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector_par.h"

/*
    Creates a Vector with the provided argument data
    
    Arguments:
        double* data - the component data of the initialized Vector. Defaults to 0 if NULL 
        int size - the size of the desired Vector

    returns 
        A pointer to a Vector type struct
*/
Vector* create_vector_par(const double* data, int size) {
    
    if(size >= 1) {

        Vector* v = malloc(sizeof(Vector));

        if(v != NULL) {
            v->size = size;

            if(data != NULL) {
                v->data = malloc(size * sizeof(double));

                #pragma omp parallel for
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
    Calculates the dot product of two vectors
    
    Arguments:
        Vector vec1 - first operand 
        Vector vec2 - second operand

    returns 
        NaN if either Vector is NULL | 
        NaN if the vectors have different sizes | 
        The value of the dot_product on success
*/
double dot_product_par(const Vector* vec1, const Vector* vec2) {
    if(vec1 == NULL || vec2 == NULL)
        return NAN;
    if(vec1->size != vec2->size)
        return NAN;

    double result = 0;

    #pragma omp parallel for reduction(+:result)
    for(int i = 0; i < vec1->size; i++) {
        result += vec1->data[i] * vec2->data[i];
    }

    return result;
}

/*
    Adds vec1 to vec2 by taking the element-wise sum of both vectors
    
    Arguments:
        Vector vec1 - first operand 
        Vector vec2 - second operand

    returns 
        NULL if either Vector is NULL | 
        NULL if the vectors have different sizes |
        NULL on failure to allocate result Vector | 
        The resulting Vector on success
*/
Vector* vector_add_par(const Vector* vec1, const Vector* vec2) {
    if(vec1 == NULL || vec2 == NULL)
        return NULL;
    if(vec1->size != vec2->size)
        return NULL;

    Vector* result = create_vector(NULL, vec1->size);

    if(result!=NULL) {
        result->size = vec1->size;
        
        #pragma omp parallel for
        for(int i = 0; i < vec1->size; i++) {
            result->data[i] = vec1->data[i] + vec2->data[i];
        }

        return result;
    }

    return NULL;
}

/*
    Modifies vec1 in-place by taking the element-wise sum of both vectors
    
    Arguments:
        Vector vec1 - destination Vector and first operand 
        Vector vec2 - second operand

    returns 
        -1 if either Vector is NULL | 
        -1 if the vectors have different sizes | 
        0 on success
*/
int vector_add_in_place_par(Vector* vec1, const Vector* vec2) {
    if(vec1 == NULL || vec2 == NULL)
        return -1;
    if(vec1->size != vec2->size)
        return -1;

    #pragma omp parallel for
    for(int i = 0; i < vec1->size; i++) {
        vec1->data[i] += vec2->data[i];
    }

    return 0;
}

/*
    Subtracts vec1 by vec2 by taking the element-wise subtraction of both vectors
    
    Arguments:
        Vector vec1 - first operand 
        Vector vec2 - second operand

    returns 
        NULL if either Vector is NULL | 
        NULL if the vectors have different sizes |
        NULL on failure to allocate result Vector | 
        The resulting Vector on success
*/
Vector* vector_sub_par(const Vector* vec1, const Vector* vec2) {
    if(vec1 == NULL || vec2 == NULL)
        return NULL;
    if(vec1->size != vec2->size)
        return NULL;

    Vector* result = create_vector(NULL, vec1->size);

    if(result!=NULL) {
        result->size = vec1->size;
        
        #pragma omp parallel for
        for(int i = 0; i < vec1->size; i++) {
            result->data[i] = vec1->data[i] - vec2->data[i];
        }

        return result;
    }

    return NULL;
}

/*
    Modifies vec1 in-place by taking the element-wise subtraction of both vectors
    
    Arguments:
        Vector vec1 - destination Vector and first operand 
        Vector vec2 - second operand

    returns 
        -1 if either Vector is NULL | 
        -1 if the vectors have different sizes | 
        0 on success
*/
int vector_sub_in_place_par(Vector* vec1, const Vector* vec2) {
    if(vec1 == NULL || vec2 == NULL)
        return -1;
    if(vec1->size != vec2->size)
        return -1;

    #pragma omp parallel for
    for(int i = 0; i < vec1->size; i++) {
        vec1->data[i] -= vec2->data[i];
    }

    return 0;
}

/*
    Modifies vec1 in-place by taking the element-wise sum of a Vector by a scalar
    
    Arguments:
        Vector vec1 - destination Vector and first operand 
        double scalar - second operand

    returns 
        -1 if the Vector is NULL |  
        0 on success
*/
int sum_scalar_par(Vector* vec1, double scalar) {
    if(vec1 == NULL)
        return -1;

    #pragma omp parallel for
    for(int i = 0; i < vec1->size; i++) {
        vec1->data[i] += scalar;
    }

    return 0;
}

/*
    Modifies vec1 in-place by taking the element-wise multiplication of a Vector by a scalar
    
    Arguments:
        Vector vec1 - destination Vector and first operand 
        double scalar - second operand

    returns 
        -1 if the Vector is NULL |  
        0 on success
*/
int multiply_scalar_par(Vector* vec1, double scalar) {
    if(vec1 == NULL)
        return -1;

    #pragma omp parallel for
    for(int i = 0; i < vec1->size; i++) {
        vec1->data[i] *= scalar;
    }

    return 0;
}

/*
    Calculates the of all the components in a Vector 
    e.g. vector_component_sum((1,1,1)) = 1 + 1 + 1 = 3
    
    Arguments:
        Vector* vec1 - first operand 
        Vector* vec2 - second operand

    returns 
        NaN if the Vector is NULL |
        The component sum on success
*/
double vector_component_sum_par(const Vector *v) {
    
    if(v!=NULL) {
        double sum = 0;

        #pragma omp parallel for reduction(+:sum)
        for(int i = 0; i < v->size; i++) {
            sum += v->data[i];
        }

        return sum;
    }

    return NAN;
}

/*
    Calculates the Euclidian Distance between two vectors
    
    Arguments:
        Vector* vec1 - first operand 
        Vector* vec2 - second operand

    returns 
        NaN if either vectors are NULL |
        NaN if the vectors have different size |  
        The Euclidian Distance on success
*/
double euclidian_distance_par(Vector* vec1, Vector* vec2) {

    if(vec1!=NULL && vec2!=NULL) {
        if(vec1->size == vec2->size) {
            double diff;
            double sum = 0;

            #pragma omp parallel for reduction(+:sum)
            for (int i = 0; i < vec1->size; i++) {
                diff = vec1->data[i] - vec2->data[i];
                sum += diff * diff;
            }

            return sqrt(sum);
        }
    }

    return NAN;
}