#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <math.h>
#include <time.h>
#include "neuron_utils.h"
#include "mat_utils.h"
#include "zero_and_one.h"

void read_neuron(FILE *file, layers **layer_list, matrix **W);
size_t (SDL_Surface* surface, layers **layer_list, matrix **W);
