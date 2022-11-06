#pragma once
#include "use.h"
SDL_Rect* rot_right(SDL_Renderer *renderer, SDL_Texture *texture, const double degree,
SDL_Surface *surface);
void rot_left(SDL_Renderer *renderer, SDL_Texture *texture, const double degree);
SDL_Surface* load_image(const char* path);
