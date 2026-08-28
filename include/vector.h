#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct {
    double* data;
    int size;
} Vector;

// Creation | Destruction

Vector* create_vector(const double* data, int size);
void free_vector(Vector* v);

// Access
double vector_get(const Vector *v, int index);
int vector_set(const Vector *v, int index, double value);

// Operations

double dot_product(const Vector* vec1, const Vector* vec2);
Vector* cross_product(const Vector* vec1, const Vector* vec2);
double triple_product(const Vector* vec1, const Vector* vec2, const Vector* vec3);
int sum_vector(Vector* vec1, Vector* vec2);
Vector* vector_add(const Vector* vec1, const Vector* vec2);
int vector_add_in_place(Vector* vec1, const Vector* vec2);
Vector* vector_sub(const Vector* vec1, const Vector* vec2);
int vector_sub_in_place(Vector* vec1, const Vector* vec2);
int sum_scalar(Vector* vec1, double scalar);
int multiply_scalar(Vector* vec1, double scalar);
double vector_component_sum(const Vector *v);
double euclidian_distance(Vector* vec1, Vector* vec2);

#endif