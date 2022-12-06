#include "use.h"
#include "math.h"
#pragma once 
int* Convolution(SDL_Surface* surface, int filtre[]);
double* gradiants(SDL_Surface *surface);
double* direction_grad(int* Gx, int* Gy, int size);
void ApplySobel(SDL_Surface *surface);
double* non_max_suppr(SDL_Surface *surface, double* theta);
char* double_threshold(SDL_Surface* s, double Low_ratio, double High_Ratio);
void hyperthesis(SDL_Surface* s);
