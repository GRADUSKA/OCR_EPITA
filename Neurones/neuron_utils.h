#pragma once

void init_layers(layers **layer_list, double *sizes);
void init_weights(matrix **W, double *sizes);
void learn(layers **layer_list, double learn_rate, double *expected_outputs,
        matrix **W);
