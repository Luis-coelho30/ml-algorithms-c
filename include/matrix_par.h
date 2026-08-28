#ifndef MATRIX_PAR_H
#define MATRIX_PAR_H

#include "matrix.h"
#include "vector_par.h"

/* Creation / destruction */

Matrix *create_matrix_par(const double* data, int rows, int cols);
Matrix* matrix_identity_par(int size);

/* Operations */

Matrix *matrix_add_par(const Matrix *a, const Matrix *b);
Matrix *matrix_subtract_par(const Matrix *a, const Matrix *b);
Matrix *matrix_multiply_par(const Matrix *a, const Matrix *b);
int matrix_scalar_multiply_par(Matrix *a, double scalar);
Vector* matrix_vec_multiply_par(const Matrix* a, const Vector* v);
Matrix *matrix_transpose_par(const Matrix *a);
void row_distance_par(const Matrix* X, const double* point, double* distances);

/* Utility */

int matrix_fill_par(Matrix* a, double value);

#endif