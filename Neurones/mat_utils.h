#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#pragma once

typedef struct{
    size_t length;
    size_t width;
    double* mat;
} matrix;
double mat_get(matrix *mat, size_t i, size_t j);
void mat_set(matrix *mat, size_t i, size_t j, double value);
void mat_add(matrix *mat1, matrix *mat2);
double* mat_mult(matrix *mat, double *list);
