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
    Gets the value in the desired position 
    
    Arguments:
        Vector* v - The vector to be accessed
        int index - the index to be accessed in v's data

    Returns:
        NaN if index is out of boundaries |
        the value on success
*/
double vector_get(const Vector *v, int index) {
    if(index >= 0 && index < v->size) {
        return v->data[index];
    }

    return NAN;
}

/*
    Sets the value in the desired position 
    
    Arguments:
        Vector* v - The vector to be accessed
        int index - the index to be accessed in v's data
        double value - the desired value

    Returns:
        -1 if index is out of boundaries |
        0 on success
*/
int vector_set(const Vector *v, int index, double value) {
    if(index >= 0 && index < v->size) {
        v->data[index] = value;
        return 0;
    }

    return -1;
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
    Adds vec1 to vec2 by taking the element-wise sum of both vectors
    
    Arguments:
        Vector vec1 - first operand 
        Vector vec2 - second operand

    returns 
        NULL if either vector is NULL | 
        NULL if the vectors have different sizes |
        NULL on failure to allocate result vector | 
        The resulting vector on success
*/
Vector* vector_add(const Vector* vec1, const Vector* vec2) {
    if(vec1 == NULL || vec2 == NULL)
        return NULL;
    if(vec1->size != vec2->size)
        return NULL;

    Vector* result = create_vector(NULL, vec1->size);

    if(result!=NULL) {
        result->size = vec1->size;
        
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
        Vector vec1 - destination vector and first operand 
        Vector vec2 - second operand

    returns 
        -1 if either vector is NULL | 
        -1 if the vectors have different sizes | 
        0 on success
*/
int vector_add_in_place(Vector* vec1, const Vector* vec2) {
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
    Subtracts vec1 by vec2 by taking the element-wise subtraction of both vectors
    
    Arguments:
        Vector vec1 - first operand 
        Vector vec2 - second operand

    returns 
        NULL if either vector is NULL | 
        NULL if the vectors have different sizes |
        NULL on failure to allocate result vector | 
        The resulting vector on success
*/
Vector* vector_sub(const Vector* vec1, const Vector* vec2) {
    if(vec1 == NULL || vec2 == NULL)
        return NULL;
    if(vec1->size != vec2->size)
        return NULL;

    Vector* result = create_vector(NULL, vec1->size);

    if(result!=NULL) {
        result->size = vec1->size;
        
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
        Vector vec1 - destination vector and first operand 
        Vector vec2 - second operand

    returns 
        -1 if either vector is NULL | 
        -1 if the vectors have different sizes | 
        0 on success
*/
int vector_sub_in_place(Vector* vec1, const Vector* vec2) {
    if(vec1 == NULL || vec2 == NULL)
        return -1;
    if(vec1->size != vec2->size)
        return -1;

    for(int i = 0; i < vec1->size; i++) {
        vec1->data[i] -= vec2->data[i];
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

/*
    Calculates the of all the components in a vector 
    e.g. vector_component_sum((1,1,1)) = 1 + 1 + 1 = 3
    
    Arguments:
        Vector* vec1 - first operand 
        Vector* vec2 - second operand

    returns 
        NaN if the vector is NULL |
        The component sum on success
*/
double vector_component_sum(const Vector *v) {
    
    if(v!=NULL) {
        double sum = 0;

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
double euclidian_distance(Vector* vec1, Vector* vec2) {

    if(vec1!=NULL && vec2!=NULL) {
        if(vec1->size == vec2->size) {
            double diff;
            double sum = 0;

            for (int i = 0; i < vec1->size; i++) {
                diff = vec1->data[i] - vec2->data[i];
                sum += diff * diff;
            }

            return sqrt(sum);
        }
    }

    return NAN;
}