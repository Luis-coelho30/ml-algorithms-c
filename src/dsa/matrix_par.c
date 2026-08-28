#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "matrix.h"
#include "vector.h"

/*
    Creates a Matrix with the provided argument data with multiple threads
    
    Arguments:
        double* data - the data to initialize the Matrix. Defaults to 0 if NULL 
        int rows - The number of rows of the Matrix 
        int cols - The number of cols of the Matrix 

    returns 
        NULL if rows or cols are lesser than zero |
        NULL in failure to allocate data for the matrix |
        A Matrix type struct
*/
Matrix* create_matrix(const double* data, int rows, int cols) {
    if(rows <= 0 || cols <= 0) {
        return NULL;
    }

    Matrix *matrix = malloc(sizeof(Matrix));

    if (matrix == NULL)
        return NULL;


    matrix->rows = rows;
    matrix->cols = cols;
    matrix->size = rows * cols;

    if(data != NULL) {
        matrix->data = malloc(matrix->size * sizeof(double));
        
        #pragma omp parallel for
        for (int i = 0; i < matrix->size; i++) {
            matrix->data[i] = data[i];
        }
    } 
    
    else {
        matrix->data = calloc(matrix->size, sizeof(double));
    }
    
    if (matrix->data == NULL) {
        free(matrix);
        return NULL;
    }

    return matrix;
}

/*
    Frees the matrix's data and itself from the heap. Must be called to avoid Memory Leaks 
    
    Arguments:
        Matrix* matrix - The Matrix to be freed
*/
void free_matrix(Matrix* matrix) {
    free(matrix->data);
    free(matrix);
}

/*
    Returns the M[row][col] value if it is within the matrix boundaries 
    
    Arguments:
        Matrix* matrix - The Matrix to extract the value from
        int row - The row to be accessed
        int col - The column to be accessed

    Returns:
        NaN if [row][col] is an invalid position
        M[row][col] if it is a valid position
*/
double matrix_get(const Matrix* m, int row, int col) {
    int pos = row * m->cols + col;
    
    if(pos > 0 && pos < m->size) {
        return m->data[row * m->cols + col];
    }

    return NAN;
}

/*
    Returns the M[row] pointer if it is within the matrix boundaries 
    
    Arguments:
        Matrix* m - The Matrix to extract the value from
        int row - The row to be accessed

    Returns:
        NULL if [row] is an invalid position
        M[row] if it is a valid position
*/
double* matrix_get_row(const Matrix* m, int row) {
    if(row >= 0 && row < m->rows) {
        return &m->data[row * m->cols];
    }
    
    return NULL;
}

/*
    Sets the M[row][col] pos to a value if it is within the matrix boundaries 
    
    Arguments:
        Matrix* matrix - The Matrix to set the value
        int row - The row to be accessed
        int col - The column to be accessed
        double value - The value to be set

    Returns:
        -1 if the position is invalid
        0 on success
*/
int matrix_set(Matrix* m, int row, int col, double value) {
    int pos = row * m->cols + col;
    
    if(pos > 0 && pos < m->size) {
        m->data[row * m->cols + col] = value;
        return 0;
    }

    return -1;
}

/*
    Multiplies matrix A by a scalar value using multiple threads
    
    Arguments:
        Matrix* matrix - The Matrix to be multiplied
        double value - The scalar value

    Returns:
        -1 if the position is invalid
        0 on success
*/
int matrix_scalar_multiply(Matrix* a, double scalar) {
    if(a!=NULL) {
        #pragma omp parallel for
        for(int i = 0; i < a->size; i++) {
            a->data[i] *= scalar; 
        }

        return 0;
    }

    return -1;
}

/*
    Adds matrix A to matrix B using multiple threads
    
    Arguments:
        Matrix* a - The first operand Matrix 
        Matrix* b - The second operand Matrix

    Returns:
        NULL if either of the matrixes are NULL
        NULL if the matrixes are of different orders
        The resulting matrix C on success
*/
Matrix* matrix_add(const Matrix* a, const Matrix* b) {
    
    if(a != NULL && b != NULL) {

        if(a->size == b->size){

            Matrix* c = create_matrix(NULL, a->rows, a->cols);
            
            if (c != NULL) {
                #pragma omp parallel for
                for(int i = 0; i < a->size; i++) {
                    c->data[i] = a->data[i] + b->data[i]; 
                }

                return c;
            }
        }
    }

    return NULL;
}

/*
    Subtracts matrix B from matrix A using multiple threads
    
    Arguments:
        Matrix* a - The first operand Matrix 
        Matrix* b - The second operand Matrix

    Returns:
        NULL if either of the matrixes are NULL
        NULL if the matrixes are of different orders
        The resulting matrix C on success
*/
Matrix* matrix_subtract(const Matrix* a, const Matrix* b) {
    
    if(a != NULL && b != NULL) {

        if(a->size == b->size){

            Matrix* c = create_matrix(NULL, a->rows, a->cols);
            
            if (c != NULL) {
                #pragma omp parallel for
                for(int i = 0; i < a->size; i++) {
                    c->data[i] = a->data[i] - b->data[i]; 
                }

                return c;
            }
        }
    }

    return NULL;
}

/*
    Multiplies matrix A by matrix B using multiple threads
    
    Arguments:
        Matrix* a - The first operand Matrix 
        Matrix* b - The second operand Matrix

    Returns:
        NULL if either of the matrixes are NULL
        NULL if the a->column != b->row
        The resulting matrix C on success
*/
Matrix* matrix_multiply(const Matrix* a, const Matrix* b) {
    
    if(a != NULL && b != NULL) {

        if(a->cols == b->rows){
            Matrix* c = create_matrix(NULL, a->rows, b->cols);

            if (c != NULL) {
                #pragma omp parallel for
                for (int i = 0; i < a->rows; i++) {
                    for (int j = 0; j < b->cols; j++) {
                        for (int k = 0; k < a->cols; k++) {
                            int index_a = i * a->cols + k;
                            int index_b = k * b->cols + j;
                            int index_c = i * c->cols + j;
                            
                            c->data[index_c] += a->data[index_a] * b->data[index_b];
                        }
                    }
                }

                return c;
            }
        }
    }

    return NULL;
}

/*
    Multiplies matrix A by a vector v using multiple threads
    
    Arguments:
        Matrix* a - The first operand Matrix 
        Vector* v - The second operand Matrix

    Returns:
        NULL if the matrix or the vector are NULL
        NULL if the a->column != v->size
        The resulting vector on success
*/
Vector* matrix_vec_multiply(const Matrix* a, const Vector* v) {
    if(a != NULL && v != NULL) {

        if(a->cols == v->size) {

            Vector* result = create_vector(NULL, a->rows);

            #pragma omp parallel for
            for (int i = 0; i < a->rows; i++) {
                for (int j = 0; j < a->cols; j++) {
                    result->data[i] += a->data[i * a->cols + j] * v->data[j];
                }
            }

            return result;
        }
    }

    return NULL;
}

/*
    Transposes the Matrix A using multiple threads
    
    Arguments:
        Matrix* a - The Matrix to be transposed

    Returns:
        NULL if the matrix is NULL
        The resulting transposed matrix
*/
Matrix* matrix_transpose(const Matrix* a) {

    if(a!=NULL) {

        Matrix* c = create_matrix(NULL, a->cols, a->rows);
            
        if (c != NULL) {
            #pragma omp parallel for
            for (int i = 0; i < a->rows; i++) {
                for (int j = 0; j < a->cols; j++) {
                    c->data[j * a->rows + i] = a->data[i * a->cols + j];
                }
            }
            
            return c;
        }
    }

    return NULL;
}

/*
    Fills the Matrix A with the desired value using multiple threads
    
    Arguments:
        Matrix* a - The Matrix to receive the value

    Returns:
        -1 if the matrix is NULL
        0 on success
*/
int matrix_fill(Matrix* a, double value) {

    if(a!=NULL) {
        #pragma omp parallel for
        for(int i = 0; i < a->size; i++) {
            a->data[i] = value;
        }

        return 0;
    }

    return -1;
}

/*
    Calculates the row distance of every row in matrix A to a point 
    
    Arguments:
        const Matrix* A - the matrix that contains the vectors
        const double* point - the desired point 
        double* distances - the distance array for each point from each row 

*/
void row_distance(const Matrix* A, const double* point, double* distances) {
    
    if(A!=NULL && point!=NULL && distances != NULL) {
        #pragma omp parallel for
        for (int j = 0; j < A->rows; j++) {
            double sum = 0.0;
            for (int f = 0; f < A->cols; f++) {
                double diff = A->data[j * A->cols + f] - point[f];
                sum += diff * diff;
            }
            distances[j] = sqrt(sum);
        }
    }
}

/*
    Creates an identity matrix of the desired size using multiple threads
    
    Arguments:
        int size - The size of the desired matrix

    Returns:
        NULL if the size is lesser than 1
        NULL on failure to create the matrix
        The identity matrix I the specified size
*/
Matrix* matrix_identity(int size) {

    if(size >= 1) {
        
        Matrix *identity = create_matrix(NULL, size, size);

        if(identity!=NULL) {
            #pragma omp parallel for
            for(int i = 0; i < size; i++)
                identity->data[i * size + i] += 1.0;

            return identity;
        }
    }

    return NULL;
}

/*
    Prints the matrix A into console
    
    Arguments:
        Matrix* a - The Matrix to be printed

*/
void matrix_print(const Matrix* a) {
    if (a != NULL) {
        for (int i = 0; i < a->rows; i++) {
            for (int j = 0; j < a->cols; j++) {
                printf("%f ", a->data[i * a->cols + j]);
            }
            printf("\n");
        }
    }
}