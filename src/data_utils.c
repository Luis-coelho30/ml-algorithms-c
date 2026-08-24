#include "data_utils.h"

#include <stdio.h>
#include <stdlib.h>

void loadFeatureMatrix(const char *path, double ***matrix, double **vector, int *rows, int *cols)
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
    *rows = 0;
    *cols = 1;

    // Count rows
    for (char *p = buffer; *p != '\0'; p++) {
        if (*p == '\n')
            (*rows)++;
    }

    // Count columns from first row
    for (char *p = buffer; *p != '\n' && *p != '\0'; p++) {
        if (*p == ',')
            (*cols)++;
    }

    // Last column is the target values
    (*cols)--;

    // Allocate space for feature matrix and target vector
    *matrix = malloc(*rows * sizeof(double *));

    for (int i = 0; i < *rows; i++)
        (*matrix)[i] = malloc(*cols * sizeof(double));

    *vector = malloc(*rows * sizeof(double));

    // Parse file to load the matrix and vector
    char *ptr = buffer;

    for (int i = 0; i < *rows; i++) {

        for (int j = 0; j < *cols; j++) {
            (*matrix)[i][j] = strtod(ptr, &ptr);

            if (*ptr == ',')
                ptr++;
        }

        // Load last column into target vector
        (*vector)[i] = strtod(ptr, &ptr);

        if (*ptr == '\n')
            ptr++;
    }

    free(buffer);
}