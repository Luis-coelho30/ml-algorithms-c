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

// Operations

double dot_product(const Vector* vec1, const Vector* vec2);
Vector* cross_product(const Vector* vec1, const Vector* vec2);
double triple_product(const Vector* vec1, const Vector* vec2, const Vector* vec3);
int sum_vector(Vector* vec1, Vector* vec2);
int sum_scalar(Vector* vec1, double scalar);
void multiply_scalar(Vector* vec1, double scalar);
double euclidian_distance(Vector* vec1, Vector* vec2);

#endif