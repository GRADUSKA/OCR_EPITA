#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Trained_Neurons/neural_network.h"

#pragma once

SDL_Surface* to_case(SDL_Surface* surface, int x, int y, int w, int h);
SDL_Surface* load_image(const char* path);
int boxes(char* path);

