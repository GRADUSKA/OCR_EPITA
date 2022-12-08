#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "use.h"

#pragma once

int test(Uint32 pixel_color, SDL_PixelFormat* format);
void fill_box(SDL_Surface* s, int w, int h, int len,Uint32* pix,int color,int number);
void fill_grid(SDL_Surface* surface,int* tab,int* origin);
int* upload(char* f_name);
int result(char* path);
