#include "neural_network.h"

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

size_t neurons(SDL_Surface *surface, layers **layer_list, matrix **W)
{
    zeroandone(surface, layer_list[0]);
    compute(layer_list, W);
    size_t max = 0;
    for(size_t i = 1; i < 9; i++)
       if(layer_list[3]->neurons[i] > layer_list[3]->neurons[max])
            max = i;
    return max + 1;
}
