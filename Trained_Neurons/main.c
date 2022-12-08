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
        "Usage:\n./main valid_data_path\n./main learn";

    errx(EXIT_FAILURE, "%s", help);
}

void exit_help_2()
{
    char* help =
        "Must train before";

    errx(EXIT_FAILURE, "%s", help);
}

void read_neuron(FILE *file, layers **layer_list, matrix **W)
{
    double div = 10000.;
    double value = 0.;
    char c = fgetc(file);
    size_t j = 0;
    for(size_t num = 0; c != '\0' && num < 3; num++)
    {
        j = 0;
        while(c != '\0' && c != '\n' && j < W[num]->width * W[num]->length)
        {
            int neg = 0;
            value = 0.;
            if(c == '-')
            {
                neg = 1;
                c = fgetc(file);
            }
            while(c != ' ' && c != '\n' && c != '\0')
            {
                value *= 10;
                value += (double)((double)c - '0');
                c = fgetc(file);
            }
            if(c == ' ')
            {
                c = fgetc(file);
            }
            W[num]->mat[j] = neg ? -value / div : value/div;
            j++;
        }
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
            int neg = 0;
            value = 0.;
            if(c == '-')
            {
                neg = 1;
                c = fgetc(file);
            }
            while(c != ' ' && c != '\n' && c != '\0')
            {
                value *= 10;
                value += (double) ((double)c - '0');
                c = fgetc(file);
            }
            if(c == ' ')
            {
                c = fgetc(file);
            }
            layer_list[num]->biases[j] = neg ? -value / div : value / div;
            j++;
        }
        if(c == '\n')
        {
            c = fgetc(file);
        }
    }
}

void write_neuron(FILE *file, layers **layer_list, matrix **W)
{
    FILE *p = file;
    for(size_t num = 0; num < 3; num++)
    {
        for(size_t i = 0; i < W[num]->width * W[num]->length; i++)
        {
            double value = W[num]->mat[i];
            if(value < 0)
            {
                value *= -1;
                fputc('-', p);
            }
            size_t j = 0;
            while(((int)value / 10))
            {
                value /= 10;
                j--;
            }
            while(j < 6)
            {
                fputc((((int) value) % 10) + '0', p);
                value *= 10;
                j++;
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
            if(value < 0)
            {
                value *= -1;
                fputc('-', p);
            }
            size_t j = 0;
            while(((int)value / 10))
            {
                value /= 10;
                j--;
            }
            while(j < 6)
            {
                fputc((((int) value) % 10) + '0', p);
                value *= 10;
                j++;
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
    sizes[1] = (size_t) 16;
    sizes[2] = (size_t) 16;
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
        printf("Result: %lu\nPercentage: %f\n", max + 1, layer_list[3]->neurons[max]);
    }
    else
    {
        size_t input_number = 0;
        const char *d_name = "training_data/";
        struct dirent* file; 
        DIR* dir;
        char dir_name[PATH_MAX + 1];
        strcpy(dir_name, d_name);
        char ki[3] = {1 + '0', '/', '\0'};
        strcat(dir_name, ki);
        dir = opendir(dir_name);
        while((file = readdir(dir)))
        {
            if(file->d_name[0] != '.')
                input_number++;
        }
        closedir(dir);

        
        for(size_t i = 0; i < 3001; i++)
        {
            size_t num = rand() % 9;
            double *expected_outputs = calloc(9, sizeof(double));
            expected_outputs[num] = 1;
            strcpy(dir_name, d_name);
            char k[3] = {num + 1 + '0', '/', '\0'};
            strcat(dir_name, k);
            dir = opendir(dir_name);
            size_t image = rand() % input_number;
            for(size_t k = 0;k < image + 3; k++)
            {
                file = readdir(dir);
                if(file->d_name[0] != '.')
                    k++;
            }
            char file_name[PATH_MAX + 1];
            strcpy(file_name, dir_name);
            strcat(file_name, file->d_name);
            transform(file_name, layer_list[0]);
            closedir(dir);
            learn(layer_list, W, expected_outputs, 0.5);

            if(!(i % 100))
            {
                printf("\n");
                printf("Input: %lu\n", num + 1);
                size_t max = 0;
                for(size_t i = 1; i < 9; i++)
                {
                    if(layer_list[3]->neurons[i] > layer_list[3]->neurons[max])
                        max = i;
                }
                printf("Result = %lu\nPercentage = %f\n",
                    max + 1, layer_list[3]->neurons[max]);
            }

            free(expected_outputs);
        }

        neuron_file = fopen("digits.txt", "w");
        write_neuron(neuron_file, layer_list, W);
    }
    for(size_t i = 0; i < 4; i++)
    {
        free(layer_list[i]);
        free(W[i]);
    }
    free(layer_list);
    free(W);

    return 1;
}
