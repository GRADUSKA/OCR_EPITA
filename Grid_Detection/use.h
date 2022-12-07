#pragma once
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
SDL_Surface* load_image(const char* path);
void save_image(char* s, SDL_Renderer* renderer, SDL_Window* win);
//void SaveScreenshot(SDL_Renderer *renderer);
