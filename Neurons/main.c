#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <math.h>
#include <time.h>
#include "neuron_utils.h"
#include "mat_utils.h"

void exit_help()
{
    char* help =
        "Usage:\n--x valid_neuron_path num num\n--x valid_neuron_path learn\n--d valid_neuron_path valid_data_path\n--d valid_neuron_path valid_data_path learn";

    errx(EXIT_FAILURE, "%s", help);
}

void init_xor(layers **input_list)
{
        double* neuron_1 = malloc(sizeof(double) * 2);
        neuron_1[0] = 0;
        neuron_1[1] = 0;
        input_list[0]->depth = 0;
        input_list[0]->neurons = neuron_1;
        input_list[0]->neuron_size = 2;

        double* neuron_2 = malloc(sizeof(double) * 2);
        neuron_2[0] = 0;
        neuron_2[1] = 1;
        input_list[1]->depth = 0;
        input_list[1]->neurons = neuron_2;
        input_list[1]->neuron_size = 2;

        double* neuron_3 = malloc(sizeof(double) * 2);
        neuron_3[0] = 1;
        neuron_3[1] = 0;
        input_list[2]->depth = 0;
        input_list[2]->neurons = neuron_3;
        input_list[2]->neuron_size = 2;

        double* neuron_4 = malloc(sizeof(double) * 2);
        neuron_4[0] = 1;
        neuron_4[1] = 1;
        input_list[3]->depth = 0;
        input_list[3]->neurons = neuron_4;
        input_list[3]->neuron_size = 2;
}

void read_neuron(FILE *file, layers **layer_list, matrix **W)
{
    size_t div = 1;
    double value = 0;
    char c = fgetc(file);
    size_t j = 0;
    for(size_t num = 0; c != '\0' && num < 3; num++)
    {
        j = 0;
        while(c != '\0' && c != '\n' && j < W[num]->width * W[num]->length)
        {
            div = 1;
            while(c != ' ' && c != '\n' && c != '\0')
            {
                value = 10 * value + c - '0';
                c = fgetc(file);
            }
            if(c == ' ')
            {
                c = fgetc(file);
            }
            W[num]->mat[j] = value / div;
            j++;
        }
        if(value != 0 && j < W[num]->width * W[num]->length)
            W[num]->mat[j] = value / div;
        if(c == '\n')
        {
            c = fgetc(file);
        }
    }
    if(c == '\n')
    {
        c = fgetc(file);
    }
    size_t num = 1;
    for(; c != '\0' && num < 4; num++)
    {
        j = 0;
        while(c != '\0' && c != '\n' && j < layer_list[num]->neuron_size)
        {
            div = 1;
            while(c != ' ' && c != '\n' && c != '\0')
            {
                value = 10 * value + c - '0';
                c = fgetc(file);
            }
            if(c == ' ')
            {
                c = fgetc(file);
            }
            layer_list[num]->biases[j] = value / div;
            j++;
        }
        if(value != 0 && j < layer_list[num]->neuron_size)
            layer_list[num]->biases[j] = value / div;
        if(c == '\n')
        {
            c = fgetc(file);
        }
    }
    if(j < layer_list[num]->neuron_size && num < 4)
        layer_list[num]->biases[j] = value / div;
}

void write_neuron(FILE *file, layers **layer_list, matrix **W)
{
    FILE *p = file;
    for(size_t num = 0; num < 3; num++)
    {
        for(size_t i = 0; i < W[num]->width * W[num]->length; i++)
        {
            double value = W[num]->mat[i];
            if(value == 0.)
            {
                fputc('0', p);
            }
            else
            {
                size_t j = 0;
                while(j < 6)
                {
                    fputc(((int) value) % 10 + '0', p);
                    value *= 10;
                    fputc(((int) value) % 10 + '0', p);
                    j++;
                }
            }
            fputc(' ', p);
        }
        fputc('\n', p);
    }
    for(size_t num = 1; num < 4; num++)
    {
        for(size_t i = 0; i < layer_list[num]->neuron_size; i++)
        {
            double value = layer_list[num]->biases[i];
            if(value == 0.)
            {
                fputc('0', p);
            }
            else
            {
                size_t j = 0;
                while(j < 6)
                {
                    fputc(((int) value) % 10 + '0', p);
                    value *= 10;
                    fputc(((int) value) % 10 + '0', p);
                    j++;
                }
            }
            fputc(' ', p);
        }
        fputc('\n', p);
    }
    fputc('\0', p);
}

void init_inputs_outputs(layers **input_list, double **expected_outputs, char *file, size_t input_number)
{
    FILE *f = fopen(file, "r");
    for(size_t i = 0; i < input_number; i++)
    {
        double *neurons = malloc(sizeof(double) * 256);
        for(size_t j = 0; j < 256; j++)
        {
            //if(j % 16 == 0)
              //  printf("\n");
            char c = fgetc(f);
            //printf("%c", c);
            neurons[j] = c - '0';
        }
        char c = fgetc(f);
        while(c != '\n' && c != '\0')
            c = fgetc(f);
        input_list[i]->neurons = neurons;
        input_list[i]->neuron_size = 256;
        input_list[i]->depth = 0;
        expected_outputs[i] = calloc(9, sizeof(double));
        expected_outputs[i][i % 9] = 1;
    }
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    if(argc != 4 && argc != 5)
        exit_help();

    size_t *sizes = malloc(sizeof(size_t) * 3);

    if(argv[1][0] == 'x')
    {
        sizes[0] = (size_t) 2;
        sizes[1] = (size_t) 4;
        sizes[2] = (size_t) 4;
        sizes[3] = (size_t) 1;
    }
    else if (argv[1][0] == 'd')
    {
        sizes[0] = (size_t) 256;
        sizes[1] = (size_t) 128;
        sizes[2] = (size_t) 64;
        sizes[3] = (size_t) 9;
    }
    else
        exit_help();

    layers **layer_list = malloc(sizeof(layers*) * 4);
    init_layers(layer_list, sizes);
    matrix **W = malloc(sizeof(matrix*) * 3);
    init_weights(W, sizes);

    FILE* neuron_file = NULL;
    neuron_file = fopen(argv[2], "r");
    if(neuron_file != NULL)
    {
        read_neuron(neuron_file, layer_list, W);
        fclose(neuron_file);
    }

    if(argv[1][0] == 'x' && argc == 5)
    {
        if((argv[3][0] != '0' && argv[3][0] != '1')
                || (argv[4][0] != '0' && argv[4][0] != '0'))
            exit_help();
        layer_list[0]->neurons[0] = argv[3][0]-'0';
        layer_list[0]->neurons[1] = argv[4][0]-'0';
        compute(layer_list, W);
        size_t max = 0;
        if(layer_list[3]->neurons[1] > layer_list[3]->neurons[0])
            max = 1;
        printf("%i xor %i = %lu\nPercentage:$: %f",
                argv[3][0]-'0', argv[4][0]-'0',
                max, layer_list[3]->neurons[max]);
    }
    else if(argv[1][0] == 'd' && argc == 4)
    {
        FILE* data_file = NULL;
        data_file = fopen(argv[3], "r");
        if (!data_file)
            exit_help();
        double* data = calloc(sizeof(double), sizes[0]);
        char c = fgetc(data_file);
        for(size_t i = 0; i < sizes[0] && fgetc(data_file+1+i) != '\0'; i++)
        {
            layer_list[0]->neurons[i] = c - '0';
            c = fgetc(data_file+1+i);
        }
        data[(size_t)c-'0'] = 1;

        fclose(data_file);
        compute(layer_list, W);
        size_t max = 0;
        for(size_t i = 1; i < sizes[2]; i++)
            if(layer_list[3]->neurons[i] > layer_list[3]->neurons[max])
                max = i;
        printf("Result: %lu\nPercentage: %f", max + 1, layer_list[3]->neurons[max]);
    }
    else if(argv[1][0] == 'x')
    {
        layers *input_1 = malloc(sizeof(layers));
        layers *input_2 = malloc(sizeof(layers));
        layers *input_3 = malloc(sizeof(layers));
        layers *input_4 = malloc(sizeof(layers));
        layers **input_list = malloc(sizeof(layers*) * 4);
        input_list[0] = input_1;
        input_list[1] = input_2;
        input_list[2] = input_3;
        input_list[3] = input_4;
        init_xor(input_list);
        double output_1[] = {0};
        double output_2[] = {1};
        double output_3[] = {1};
        double output_4[] = {0};
        double **expected_outputs = malloc(sizeof(double*) * 4);
        expected_outputs[0] = output_1;
        expected_outputs[1] = output_2;
        expected_outputs[2] = output_3;
        expected_outputs[3] = output_4;

        for(size_t i = 0; i < 1000000000; i++)
        {
            printf("------------------------------------------------\n");
            learn(input_list, layer_list, 0.4, expected_outputs, W, 4, i);
            printf("\n");
        }
        for(size_t i = 0; i < 4; i++)
        {
            for(size_t j = 0; j < W[i]->length * W[i]->width; j++)
                printf("%f ", W[i]->mat[j]);
            printf("\n");
        }

        neuron_file = fopen(argv[2], "w");
        write_neuron(neuron_file, layer_list, W);
    }
    else
    {
        size_t input_number = 9;
        layers **input_list = malloc(sizeof(layers*) * input_number);
        for(size_t i = 0; i < input_number; i++)
        {
            input_list[i] = malloc(sizeof(layers*));
        }
        double **expected_outputs = malloc(sizeof(double*) * input_number);

        init_inputs_outputs(input_list, expected_outputs, argv[3], input_number);

        
        for(size_t i = 0; i < 1000000000; i++)
        {
            printf("------------------------------------------------\n");
            learn(input_list, layer_list, 0.01, expected_outputs, W, input_number, i);
            printf("\n");
        }
        
        /*
        for(size_t i = 0; i < 4; i++)
        {
            for(size_t j = 0; j < W[i]->length * W[i]->width; j++)
                printf("%f ", W[i]->mat[j]);
            printf("\n");
        }

        neuron_file = fopen(argv[2], "w");
        write_neuron(neuron_file, layer_list, W);
        */
    }

    return 1;
}
