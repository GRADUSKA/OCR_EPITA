#pragma once
#include "mat_utils.h"

typedef struct{
    size_t  depth;
    size_t  input_size;
    size_t  neuron_size;
    double* neurons;
    double* biases;
} layers;

void init_layers(layers **layer_list, size_t *sizes);
void init_weights(matrix **W, size_t *sizes);
void compute(layers **layer_list, matrix **W);
void learn(layers **input_list, layers **layer_list, double learn_rate, 
        double **expected_outputs, matrix **W, size_t input_number);
