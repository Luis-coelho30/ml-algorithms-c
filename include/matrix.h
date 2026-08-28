#ifndef MATRIX_H
#define MATRIX_H

typedef struct Vector Vector;

typedef struct {
    double* data;
    int rows;
    int cols;
    int size;
} Matrix;

/* Creation / destruction */

Matrix *create_matrix(const double* data, int rows, int cols);
void free_matrix(Matrix *matrix);
Matrix* matrix_identity(int size);

/* Access */

double matrix_get(const Matrix *m, int row, int col);
double* matrix_get_row(const Matrix* m, int row);
int matrix_set(Matrix *m, int row, int col, double value);

/* Operations */

Matrix *matrix_add(const Matrix *a, const Matrix *b);
Matrix *matrix_subtract(const Matrix *a, const Matrix *b);
Matrix *matrix_multiply(const Matrix *a, const Matrix *b);
Matrix *matrix_scalar_multiply(Matrix *a, double scalar);
Vector* matrix_vec_multiply(const Matrix* a, const Vector* v);
Matrix *matrix_transpose(const Matrix *a);
void row_distance(const Matrix* X, const double* point, double* distances);

/* Utility */

void matrix_print(const Matrix *a);
int matrix_fill(Matrix* a, double value);

#endif