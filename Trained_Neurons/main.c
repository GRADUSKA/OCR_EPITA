#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include "neuron_utils.h"
#include "mat_utils.h"
#include "zero_and_one.h"

#define PATH_MAX 4096

void exit_help()
{
    char* help =
        "Usage:\nvalid_data_path\nlearn";

    errx(EXIT_FAILURE, "%s", help);
}

void exit_help_2()
{
    char* help =
        "Must train before";

    errx(EXIT_FAILURE, "%s", help);
}

void exit_help_3()
{
    char* help =
        "No training data";

    errx(EXIT_FAILURE, "%s", help);
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

int main(int argc, char** argv)
{
    srand(time(NULL));

    if(argc != 2)
        exit_help();

    size_t *sizes = malloc(sizeof(size_t) * 4); 
    sizes[0] = (size_t) 256;
    sizes[1] = (size_t) 128;
    sizes[2] = (size_t) 64;
    sizes[3] = (size_t) 9;

    layers **layer_list = malloc(sizeof(layers*) * 4);
    init_layers(layer_list, sizes);
    matrix **W = malloc(sizeof(matrix*) * 3);
    init_weights(W, sizes);

    FILE* neuron_file = fopen("digits.txt", "r");
    if(neuron_file)
    {
        read_neuron(neuron_file, layer_list, W);
        fclose(neuron_file);
    }

    if(argv[1][0] != 'l')
    {
        if(!neuron_file)
            exit_help_2();
        transform(argv[1], layer_list[0]);
        compute(layer_list, W);
        size_t max = 0;
        for(size_t i = 1; i < sizes[3]; i++)
            if(layer_list[3]->neurons[i] > layer_list[3]->neurons[max])
                max = i;
        printf("Result: %lu\nPercentage: %f", max + 1, layer_list[3]->neurons[max]);
    }
    else
    {
        size_t input_number = 0;
        DIR* d;
        const char *d_name = "training_data/";
        d = opendir(d_name);
        if(!d)
            exit_help_3();
        struct dirent* dir;
        struct dirent* file;
        dir = readdir(d);
        if(dir->d_type)
        {
            DIR* di;
            char dir_name[PATH_MAX + 1];
            strcpy(dir_name, d_name);
            strcat(dir_name, dir->d_name);
            strcat(dir_name, "/");
            di = opendir(dir_name);
            while((file = readdir(di)))
                input_number++;
            closedir(di);
        }
        closedir(d);
        input_number *= 9;
        layers **input_list = malloc(sizeof(layers*) * input_number);
        double **expected_outputs = malloc(sizeof(double*) * input_number);
        for(size_t i = 0; i < input_number; i++)
        {
            input_list[i] = malloc(sizeof(layers*));
            expected_outputs[i] = calloc(9, sizeof(double));
            expected_outputs[i][i % 9] = 1;
        }
        d = opendir(d_name);
        size_t num = 0;
        while((dir = readdir(d)))
        {
            DIR* di;
            char dir_name[PATH_MAX + 1];
            strcpy(dir_name, d_name);
            strcat(dir_name, dir->d_name);
            strcat(dir_name, "/");
            di = opendir(dir_name);
            if(dir->d_type)
            {
                size_t i = 0;
                while((file = readdir(di)))
                {
                    char file_name[PATH_MAX + 1];
                    strcpy(file_name, dir_name);
                    strcat(file_name, file->d_name);
                    transform(file_name, input_list[i * 9 + num]);
                    i++;
                }
                num++;
            }
            closedir(di);
        }
        closedir(d);
        for(size_t i = 0; i < 20; i++)
            learn(input_list, layer_list, 0.01, expected_outputs, W, input_number, i); 
        neuron_file = fopen("digits.txt", "w");
        write_neuron(neuron_file, layer_list, W);
    }

    return 1;
}
