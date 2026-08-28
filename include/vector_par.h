#ifndef VECTOR_PAR_H
#define VECTOR_PAR_H

#include <stddef.h>
#include "vector.h"

// Creation | Destruction

Vector* create_vector_par(const double* data, int size);

// Operations

double dot_product_par(const Vector* vec1, const Vector* vec2);
Vector* vector_add_par(const Vector* vec1, const Vector* vec2);
int vector_add_in_place_par(Vector* vec1, const Vector* vec2);
Vector* vector_sub_par(const Vector* vec1, const Vector* vec2);
int vector_sub_in_place_par(Vector* vec1, const Vector* vec2);
int sum_scalar_par(Vector* vec1, double scalar);
int multiply_scalar_par(Vector* vec1, double scalar);
double vector_component_sum_par(const Vector *v);
double euclidian_distance_par(Vector* vec1, Vector* vec2);

#endif