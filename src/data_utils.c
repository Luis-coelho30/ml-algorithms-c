#include "data_utils.h"

#include <stdio.h>
#include <stdlib.h>

void loadFeatureMatrix(const char *path, Matrix **X, Vector **y)
{
    // Get file size
    FILE *file = fopen(path, "rb");

    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Load entire file into memory as a buffer
    char *buffer = malloc(file_size + 1);

    if (buffer == NULL) {
        perror("Failed to allocate buffer");
        fclose(file);
        return;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    buffer[bytes_read] = '\0';

    fclose(file);

    // Determine dimensions
    int rows = 0;
    int cols = 1;

    if (bytes_read > 0 && buffer[bytes_read - 1] == '\n')
        buffer[bytes_read - 1] = '\0';


    // Count rows
    for (char *p = buffer; *p != '\0'; p++) {
        if (*p == '\n')
            rows++;
    }

    rows++;

    // Count columns from first row
    for (char *p = buffer; *p != '\n' && *p != '\0'; p++) {
        if (*p == ',')
            cols++;
    }

    // Last column is the target values
    cols--;

    // Temp flat buffers for parsing
    double *X_temp = malloc(rows * cols * sizeof(double));
    double *y_temp = malloc(rows * sizeof(double));

    if (X_temp == NULL || y_temp == NULL) {
        perror("Failed to allocate temp parse buffers");
        free(buffer);
        free(X_temp);
        free(y_temp);
        return;
    }


    char *ptr = buffer;
 
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            X_temp[i * cols + j] = strtod(ptr, &ptr);
            if (*ptr == ',')
                ptr++;
        }
 
        y_temp[i] = strtod(ptr, &ptr);
 
        if (*ptr == '\n')
            ptr++;
    }
 
    *X = create_matrix(X_temp, rows, cols);
    *y = create_vector(y_temp, rows);
 
    free(X_temp);
    free(y_temp);
    free(buffer);
}