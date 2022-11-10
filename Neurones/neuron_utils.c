#include "mat_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* neuron_utils.c */

typedef struct{
    size_t depth;
    size_t input_size;
    size_t neuron_size;
    double* neurons;
    double* biases;
} layers;

double get_neuron(layers *layer, size_t i)
{
    return *((layer->neurons) + i);
}

void init_layers(layers **layer_list, size_t *sizes)
{
    layers *input_layer = malloc(sizeof(layers));
    layers *hidden_layer1 = malloc(sizeof(layers));
    layers *hidden_layer2 = malloc(sizeof(layers));
    layers *output_layer = malloc(sizeof(layers));

    double *in_list = calloc(sizes[0], sizeof(double));
    double *hid_list1 = calloc(sizes[1], sizeof(double));
    double *hid_list2 = calloc(sizes[1], sizeof(double));
    double *out_list = calloc(sizes[2], sizeof(double));

    double *b1 = malloc(sizes[1] * sizeof(double));
    double *b2 = malloc(sizes[1] * sizeof(double));
    double *b3 = malloc(sizes[2] * sizeof(double));

    for(size_t out = 0; out < sizes[1]; out++){
        b1[out] = ((double)rand() + (double)rand()) / ((double)RAND_MAX);
        b2[out] = ((double)rand() + (double)rand()) / ((double)RAND_MAX);
    }

    for(size_t out = 0; out < sizes[2]; out++){
        b3[out] = ((double)rand() + (double)rand()) / ((double)RAND_MAX);
    }

    input_layer->depth = 0;
    hidden_layer1->depth = 1;
    hidden_layer2->depth = 2;
    output_layer->depth = 3;

    input_layer->input_size = 0;
    hidden_layer1->input_size = *sizes;
    hidden_layer2->input_size = *(sizes + 1);
    output_layer->input_size = *(sizes + 1);

    input_layer->neuron_size = sizes[0];
    hidden_layer1->neuron_size = sizes[1];
    hidden_layer2->neuron_size = sizes[1];
    output_layer->neuron_size = sizes[2];

    input_layer->neurons = in_list;
    hidden_layer1->neurons = hid_list1;
    hidden_layer2->neurons = hid_list2;
    output_layer->neurons = out_list;

    hidden_layer1->biases = b1;
    hidden_layer2->biases = b2;
    output_layer->biases = b3;

    *layer_list = input_layer;
    *(layer_list + 1) = hidden_layer1;
    *(layer_list + 2) = hidden_layer2;
    *(layer_list + 3) = output_layer;
}

void init_weight(double *w, size_t *sizes, size_t i)
{
    for(size_t length = 0; length < *(sizes + ((i + 1) / 2)); length++){
        for(size_t width = 0; width < *(sizes + i / 2); width++)
        {
            w[length * (*(sizes + i / 2)) + width] =
                ((double)rand() + (double)rand()) / ((double)RAND_MAX);
        }
    }
}

void init_weights(matrix **W, size_t *sizes)
{
    size_t memory_1 = *(sizes + 1) * *sizes;
    size_t memory_2 = *(sizes + 1) * *(sizes + 1);
    size_t memory_3 = *(sizes + 2) * *(sizes + 1);
    double *mat1 = malloc(memory_1 * sizeof(double));
    double *mat2 = malloc(memory_2 * sizeof(double));
    double *mat3 = malloc(memory_3 * sizeof(double));
    init_weight(mat1, sizes, 1);
    init_weight(mat2, sizes, 2);
    init_weight(mat3, sizes, 3);
    matrix *W1 = malloc(sizeof(matrix));
    matrix *W2 = malloc(sizeof(matrix));
    matrix *W3 = malloc(sizeof(matrix));
    *W1 = (matrix) {.mat = mat1,.length = *(sizes + 1),.width = *sizes};
    *W2 = (matrix) {.mat = mat2,.length = *(sizes + 1),.width = *(sizes + 1)};
    *W3 = (matrix) {.mat = mat3,.length = *(sizes + 2),.width = *(sizes + 1)};

    *W = W1;
    *(W + 1) = W2;
    *(W + 2) = W3;
}

void copy_neurons(layers *layer, double *copy)
{
    for(size_t neuron = 0; neuron < layer->neuron_size; neuron++)
        copy[neuron] = get_neuron(layer, neuron);
}

double sigmoid(double z)
{
    return 1. / (1. + (double)exp(-z));
}

double get_bias(layers *layer, size_t i)
{
    return layer->biases[i];
}

void weight_inputs(matrix *W, layers *in_layer, layers *out_layer)
{
    for(size_t out = 0; out < W->length; out++){
        for(size_t in = 0; in < W->width; in++)
        {
            out_layer->neurons[out]
                += get_neuron(in_layer, in) * W->mat[out * W->width + in];
        }
    }
}

void bias_neurons_and_sigmoid(layers *layer)
{
    for(size_t neuron = 0; neuron < layer->neuron_size; neuron++){
        layer->neurons[neuron] =
            sigmoid(get_neuron(layer, neuron) + get_bias(layer, neuron));
    }
}

void compute_next_layer(layers *input, matrix *W, layers *output)
{
    for(size_t out = 0; out < output->neuron_size; out++){
        double neuron = 0;
        for(size_t in = 0; in < output->input_size; in ++){
            neuron += get_neuron(input, in) * W->mat[out * W->width + in];
        }
        *(output->neurons + out) = sigmoid(neuron - get_bias(output, out));
    }
}

double cost(layers *outputs, double *expected_output, size_t output)
{
    double error = expected_output[output] - get_neuron(outputs, output);
    return error*error;
}

double cost_derivative(size_t output_change, layers *layer,
        double *expected_outputs){
    return 2.*(expected_outputs[output_change] - layer->neurons[output_change]);
}

double sigmoid_derivative(double z)
{
    double res = 1. / (1. + (double)exp(-z));
    return res * (1. - res);
}

void compute(layers **layer_list, matrix **W)
{
    for(size_t i = 0; i < 3; i++)
        compute_next_layer(layer_list[i], W[i], layer_list[i+1]);
}

void calculate_outputs(layers **layer_list, matrix **W, double **weighted_inputs,
        double *weighted_outputs)
{
    for(size_t i = 0; i < 2; i++)
    {
        weight_inputs(W[i], layer_list[i], layer_list[i+1]);
        copy_neurons(layer_list[i+1], weighted_inputs[i]);
        bias_neurons_and_sigmoid(layer_list[i+1]);
    }
    weight_inputs(W[2], layer_list[2], layer_list[3]);
    copy_neurons(layer_list[3], weighted_outputs);
    bias_neurons_and_sigmoid(layer_list[3]);
}
void calculate_values(layers *output_layer, double *expected_output,
        double *weighted_output, double *neuron_values)
{
    for(size_t neuron = 0; neuron < output_layer->neuron_size; neuron++){
        neuron_values[neuron] =
            cost_derivative(neuron, output_layer, expected_output)
            * sigmoid_derivative(weighted_output[neuron]);
    }
}

void calculate_hidden_values(double *neuron_values, matrix **W,
        layers *hidden_layer, double *weighted_hidden,
        double *hidden_neuron_values)
{
    for(size_t hidden_neuron = 0; hidden_neuron < hidden_layer->neuron_size
            ; hidden_neuron++){
        double hidden_value = 0;

        for(size_t output_neuron = 0;
                output_neuron < W[hidden_layer->depth]->length; output_neuron++)
        {
            hidden_value +=
                W[hidden_layer->depth]->
                mat[hidden_neuron * W[hidden_layer->depth]->width + output_neuron]
                * neuron_values[output_neuron];
        }

        hidden_neuron_values[hidden_neuron] =
            hidden_value * sigmoid_derivative(weighted_hidden[hidden_neuron]);
    }
}

/* a revoir */

void update_gradient(matrix **W, double **biases, layers *in_layer,
        layers *output_layer, double *neuron_values)
{
    for(size_t out = 0; out < output_layer->neuron_size; out++)
    {
        for(size_t in = 0; in < in_layer->neuron_size; in++)
        {
            W[in_layer->depth]->mat[out * in_layer->neuron_size + in] +=
                in_layer->neurons[in] * neuron_values[out];
        }
        biases[in_layer->depth][out] += neuron_values[out];
    }
}

void update_all_gradients(layers **input, matrix **W,
        double *expected_output, matrix **grad_w, double **grad_bias)
{
    double* weighted_1 = malloc(sizeof(double) * input[1]->neuron_size);
    double* weighted_2 = malloc(sizeof(double) * input[1]->neuron_size);
    double** weighted_neurons =
        malloc((sizeof(double) * input[1]->neuron_size) * 2);
    weighted_neurons[0] = weighted_1;
    weighted_neurons[1] = weighted_2;
    double* weighted_outputs = malloc(sizeof(double) * input[3]->neuron_size);
    calculate_outputs(input, W, weighted_neurons, weighted_outputs);

    double *neuron_values = malloc(sizeof(double) * input[3]->neuron_size);
    calculate_values(input[3], expected_output,
            weighted_outputs, neuron_values);
    update_gradient(grad_w, grad_bias,
            input[2], input[3], neuron_values);
    for(size_t i = 2; i > 0; i--)
    {
        double *hidden_neuron_values =
            calloc(sizeof(double), input[i]->neuron_size);
        calculate_hidden_values(neuron_values, W, input[i],
                weighted_neurons[i-1], hidden_neuron_values);
        update_gradient(grad_w, grad_bias,
                input[i-1], input[i], hidden_neuron_values);
        neuron_values = malloc(sizeof(double) * input[i]->neuron_size);
        neuron_values = hidden_neuron_values;
    }
}

void apply_gradients(double learn_rate, layers **layer, matrix **W,
        matrix **grad_w, double **grad_bias)
{
    for(size_t i = 0; i < 3; i++)
    {
        for(size_t out = 0; out < layer[i+1]->neuron_size; out++)
        {
            for(size_t in = 0; in < layer[i]->neuron_size; in++)
            {
                //printf("weight grad n°%lu [%lu][%lu] = %f\n",
                  //      i, out, in, grad_w[i]->mat[out * W[i]->width + in]);
                W[i]->mat[out * W[i]->width + in] +=
                    grad_w[i]->mat[out * W[i]->width + in] * learn_rate;
            }
            //printf("bias grad n°%lu [%lu] = %f\n", i, out, grad_bias[i][out]);
            layer[i+1]->biases[out] += grad_bias[i][out] * learn_rate;
        }
    }
}

int grad_changes(matrix **grad_w, double **grad_bias, layers **layer_list)
{
    for(size_t i = 0; i < 3; i++)
    {
        for(size_t out = 0; out < layer_list[i+1]->neuron_size; out++)
        {
            for(size_t in = 0; in < layer_list[i]->neuron_size; in++)
            {
                if(grad_w[i]->mat[out * layer_list[i]->neuron_size + in] > 0.01)
                    return 0;
            }
            if(grad_bias[i][out] > 0.01)
               return 0;
        }
    }
    return 1;
}

void shuffle(matrix **grad_w, double **grad_b, layers **layer_list, double costs)
{
    for(size_t i = 0; i < 3; i++)
    {
        for(size_t out = 0; out < layer_list[i+1]->neuron_size; out++)
        {
            for(size_t in = 0; in < layer_list[i]->neuron_size; in++)
            {
                grad_w[i]->mat[out * layer_list[i]->neuron_size + in] *= costs * 10.;
            }
            grad_b[i][out] *= costs * 10.;
        }
    }
}

void learn(layers **input_list, layers **layer_list, double learn_rate,
        double **expected_outputs, matrix **W, size_t input_number)
{
    size_t input = 0;
    matrix **grad_w = malloc(sizeof(matrix) * 3);
    double **grad_bias =
        malloc((sizeof(double) * layer_list[1]->neuron_size) * 3);
    for(size_t i = 0; i < 3; i++)
    {
        matrix *m = calloc(sizeof(matrix),1);
        m->mat = calloc(sizeof(double), (W[i]->width * W[i]->length));
        for(size_t j = 0; j < W[i]->width * W[i]->length; j++)
            m->mat[j] = (double)rand() / (double)RAND_MAX;
        grad_w[i] = m;
        double *b = calloc(sizeof(double), layer_list[1]->neuron_size);
        grad_bias[i] = b;
    }
    while(input < input_number)
    {
        for(size_t i = 1; i < 4; i++)
        {
            for(size_t neuron = 0; neuron < layer_list[i]->neuron_size; neuron++)
                layer_list[i]->neurons[neuron] = 0;
        }

        printf("\n");
        layer_list[0]->neurons = input_list[input]->neurons;
        printf("input = %i %i\n",
                (int)layer_list[0]->neurons[0], (int)layer_list[0]->neurons[1]);
        update_all_gradients(layer_list, W, expected_outputs[input],
                grad_w, grad_bias);
        size_t max = 0;
        if(0.5 < layer_list[3]->neurons[0])
            max = 1;
        printf("result = %lu\npercentage = %f\n",
                max, layer_list[3]->neurons[0]);
        input += 1;
    }
    double costs = 0;
    for(size_t i = 0; i < layer_list[3]->neuron_size; i++)
    {
        costs += cost(layer_list[3], expected_outputs[3], i);
    }
    if(costs > 0.4 && grad_changes(grad_w, grad_bias, layer_list))
            shuffle(grad_w, grad_bias, layer_list, costs);
    apply_gradients(learn_rate/input_number, layer_list, W, grad_w, grad_bias);
}
