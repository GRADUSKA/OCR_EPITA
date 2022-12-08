#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "neuron_utils.h"
void transform(const char* path, layers* layer);
int which(int c, int surf, int max, Uint32* pixels, SDL_PixelFormat* format, int x, int y);
int test11(SDL_Surface* surface);
void zeroandone(SDL_Surface* surface, layers* layer);
