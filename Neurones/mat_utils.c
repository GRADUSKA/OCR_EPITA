#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

/* mat_utils.c */

typedef struct{
    size_t length;
    size_t width;
    double mat[];
} matrix;

double mat_get(matrix *mat, size_t i, size_t j)
{
    return mat->mat[i * mat->width + j];
}

void mat_set(matrix *mat, size_t i, size_t j, double value)
{
    mat->mat[i * mat->width + j] = value;
}

void mat_add(matrix *mat1, matrix *mat2)
{
    for(size_t i = 0; i < mat1->length; i++){
        for(size_t j = 0; j < mat1->width; j++)
            mat1->mat[i * mat1->width + j] += mat_get(mat2, i, j);
    }
}

double* mat_mult(matrix *mat, double *list)
{
    double *res = malloc(mat->length);

    for(size_t i = 0; i < mat->length; i++){
        for(size_t j = 0; j < mat->width; j++){

            res[i] += mat_get(mat, i, j) * list[j];
        }
    }
    return res;
}
