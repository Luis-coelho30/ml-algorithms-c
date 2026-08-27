#include <math.h>
#include <stdio.h>
#include "vector.h"

typedef struct {
    double* data;
    int rows;
    int cols;
    int size;
} Matrix;

/*
    Creates a Matrix with the provided argument data
    
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
        
        for (int i = 0; i < matrix->size; i++) {
            matrix->data[i] = data[i];
        }
    } 
    
    else {
        matrix->data = calloc(matrix->size * sizeof(double));
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
    Multiplies matrix A by a scalar value 
    
    Arguments:
        Matrix* matrix - The Matrix to be multiplied
        double value - The scalar value

    Returns:
        -1 if the position is invalid
        0 on success
*/
int matrix_scalar_multiply(Matrix* a, double scalar) {
    if(a!=NULL) {
        for(int i = 0; i < a->size; i++) {
            a->data[i] *= scalar; 
        }

        return 0;
    }

    return -1;
}

/*
    Adds matrix A to matrix B  
    
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

            Matrix* c = createMatrix(NULL, a->rows, a->cols);
            
            if (c != NULL) {
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
    Subtracts matrix B from matrix A  
    
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

            Matrix* c = createMatrix(NULL, a->rows, a->cols);
            
            if (c != NULL) {
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
    Multiplies matrix A by matrix B  
    
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
            Matrix* c = createMatrix(NULL, a->rows, b->cols);

            if (c != NULL) {
                int index_a, index_b, index_c;

                for (int i = 0; i < a->rows; i++) {
                    for (int j = 0; j < b->cols; j++) {
                        for (int k = 0; k < a->cols; k++) {
                            index_a = i * a->cols + k;
                            index_b = k * b->cols + j;
                            index_c = i * c->cols + j;
                            
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
    Multiplies matrix A by a vector v  
    
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
    Transposes the Matrix A  
    
    Arguments:
        Matrix* a - The Matrix to be transposed

    Returns:
        NULL if the matrix is NULL
        The resulting transposed matrix
*/
Matrix* matrix_transpose(const Matrix* a) {

    if(a!=NULL) {

        Matrix* c = createMatrix(NULL, a->cols, a->rows);
            
        if (c != NULL) {
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
    Fills the Matrix A with the desired value 
    
    Arguments:
        Matrix* a - The Matrix to receive the value

    Returns:
        -1 if the matrix is NULL
        0 on success
*/
int matrix_fill(Matrix* a, double value) {

    if(a!=NULL) {
        for(int i = 0; i < a->size; i++) {
            a->data[i] = value;
        }

        return 0;
    }

    return -1;
}

/*
    Creates an identity matrix of the desired size 
    
    Arguments:
        int size - The size of the desired matrix

    Returns:
        NULL if the size is lesser than 1
        NULL on failure to create the matrix
        The identity matrix I the specified size
*/
Matrix* matrix_identity(int size) {

    if(size >= 1) {
        
        Matrix *identity = createMatrix(NULL, size, size);

        if(identity!=NULL) {
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