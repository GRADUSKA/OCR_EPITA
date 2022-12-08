#include "use.h"
#pragma once
Uint32 SDL_LirePixel(SDL_Surface* surface, int x, int y);
void SDL_EcrirePixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
SDL_Surface* Rotation_shearing(SDL_Surface* image, double angle);
float get_angle(char* s);
