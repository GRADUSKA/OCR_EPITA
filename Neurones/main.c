#include <stdio.h>
#include <stdlin.h>
#include <string.h>
#include <err.h>
#include "neuron_utils.h"

void exit_help()
{
    char* help =
        "Usage:\n
        --xor valid_neuron_path num num\n
        --xor valid_neuron_path learn\n
        --digit valid_neuron_path valid_data_path\n
        --digit valid_neuron_path valid_data_path learn";

    errx(EXIT_FAILURE, "%s", help);
}

void init_xor(layer **input_list)
{
    for(size_t i = 0; i < 4; i++)
    {
        double* neuron = {i/2, (i+1)/2 - (i/3)*2}
        input_list[i]->depth = 0;
        input_list[i]->neurons = neuron;
        input_list[i]->neuron_size = 2;
    }
}

void read_neuron(FILE *file, layer **layer_list, matrix **W, size_t *sizes)
{
    size_t i = 0;
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
                div *= 10;
                i++;
                c = fgetc(file + i);
            }
            if(c == ' ')
            {
                i++;
                c = fgetc(file + i);
            }
            W[num]->mat[j] = value / div;
            j++;
        }
        if(value != 0 && i < W[num]->width * W[num]->length)
            W[num]->mat[j] = value / div;
        if(c == '\n')
        {
            i++;
            c = fgetc(file + i);
        }
    }
    if(c == '\n')
    {
        i++;
        c = fgetc(file + i);
    }
    for(size_t num = 1; c != '\0' && num < 4; num++)
    {
        j = 0; 

        while(c != '\0' && c != '\n' && j < layer_list[num]->neuron_size)    
        {
            div = 1;
            while(c != ' ' && c != '\n' && c != '\0')
            {
                value = 10 * value + c - '0';
                div *= 10;
                i++;
                c = fgetc(file + i);
            }
            if(c == ' ')
            {
                i++;
                c = fgetc(file + i);
            }
            layer_list[num]->biases[j] = value / div;
            j++;
        }
        if(value != 0 && i < layer_list[num]->neuron_size)
            layer_list[num]->biases[j] = value / div;
        if(c == '\n')
        {
            i++;
            c = fgetc(file + i);
        }
    }
    if(j < layer_list[num]->neuron_size && num < 4)
        layer_list[num]->biases[j] = value / div;
}

void write_neuron(FILE *file, layer **layer_list, matrix **W)
{
    FILE *p = file;
    for(size_t num = 0; num < 3; num++)
    {
        for(size_t i = 0; i < W[num]->width * W[num]->length; i++)
        {
            double value = W[num]->mat[i];
            if(value == 0)
            {
                fputc(&p, '0');
                p++;
            }
            else
            {
                while(((int) value) != value)
                {
                    value *= 10;
                    fputc(&p, ((int) value) % 10 + '0');
                    p++;
                }
            }
            fputc(&p, ' ');
            p++;
        }
        fputc(&p, '\n');
        p++;
    }
    for(size_t num = 1; num < 4; num++)
    {
        for(size_t i = 0; i < layer_list[num]->neuron_size; i++)
        {
            double value = layer_list[num]->biases[i];
            if(value == 0)
            {
                fputc(&p, '0');
                p++;
            }
            else
            {
                while(((int) value) != value)
                {
                    value *= 10;
                    fputc(&p, ((int) value) % 10 + '0');
                    p++;
                }
            }
            fputc(&p, ' ');
            p++;
        }
        fputc(&p, '\n');
        p++;
    }
    fputc(&p, '\0');

}

void main(size_t argc, char** argv)
{
    if(argc != 4 && argc != 5)
        exit_help();

    size_t *sizes[3];

    if(argv[1] == "xor")
    {
        sizes[0] = 2;
        sizes[1] = 2;
        sizes[2] = 2;
    }
    else if (argv[1] == "digit")
    {
        sizes[0] = 256;
        sizes[1] = 256;
        size[2] = 10;
    }
    else
        exit_help();

    layer **layer_list[4];
    init_layers(&layer_list, &sizes);
    matrix **W[3];
    init_weights(&W, &sizes);

    FILE* neuron_file = NULL;
    neuron_file = fopen(argv[2], "r");
    if(neuron_file != NULL)
    {
        read_neuron(&neuron_file, &layer_list, &sizes);
        fclose(neuron_file);
    }

    if(argv[1] == "xor" && argc == 5)
    {
        if(argv[3] != '0' && argv[3] != '1' 
                || argv[4] != '0' && argv[4] != '0')
            exit_help();
        layer_list[0]->neurons = {argv[3]-'0', argv[4]-'0'};
        compute(&layer_list, &W);
        size_t max = 0;
        if(layer_list[3]->neurons[1] > layer_list[3]->neurons[0])
            max = 1;
        printf("%i xor %i = %i\n
                Percentage:$: %f",
                argv[3]-'0', argv[4]-'0', max, layer_list[3]->neurons[max]);
    }
    else if(argc == 4)
    {
        FILE* data_file = NULL;
        data_file = fopen(argv[3], "r");
        if (!data_file)
            exit_help();
        double* data = malloc(sizeof(double) * sizes[0]);
        char c = fgetc(data);
        for(size_t i = 0; i < sizes[0] && c != '\0'; i++)
        {
            layer_list[0]->neurons[i] = c - '0';
            c = fgetc(data+1+i);
        }
        fclose(data_file);
        compute(&layer_list, &W);
        size_t max = 0;
        for(size_t i = 1; i < sizes[3]; i++)
            if(layer_list[3]->neurons[i] > layer_list[3]->neurons[max])
                max = i;
        printf("Result: %i\n
                Percentage: %f", max + 1, layer_list[3]->neurons[max]);
    }
    else if(argv[1] == "xor")
    {
        layer *input_1;
        layer *input_2;
        layer *input_3;
        layer *input_4;
        layer **input_list = {input_1, input_2, input_3, input_4};
        init_xor(&input_list);
        double *output_1 = {1, 0};
        double *output_2 = {0, 1};
        double *output_3 = {0, 1};
        double *output_4 = {1, 0};
        double **expected_outputs = {output_1, output_2, output_3, output_4};

        for(size_t i = 0; i < 1; i++)
            learn(&input_list, &layer_list, 0.1, &expected_outputs, &W, 4);

        neuron_file = fopen(argv[2], "w");
        write_neuron(&neuron_file, &layer_list, &W);
    }
    else
    {
        // TODO:
        // - Lire le fichier de données
        // - Faire 2 listes, les input et les resultats
        // - Learn sur tous   
    }
}
