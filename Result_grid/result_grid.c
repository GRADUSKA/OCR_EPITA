#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include "use.h"

void draw(SDL_Renderer* renderer, int w, int h)
{
    // Clears the renderer (sets the background to black).
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Sets the color for drawing operations to white.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draws the two diagonals.
    SDL_RenderDrawLine(renderer, 0, 0, w, h);
    SDL_RenderDrawLine(renderer, 0, h, w, 0);

    // Updates the display.
    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("RESULT", 0, 0, 100, 100,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer
    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* s = load_image(argv[1]);
    SDL_Texture* t =  SDL_CreateTextureFromSurface(renderer, s);

    draw(renderer,s->w,s->h);

    if(SDL_SaveBMP(s, "test_grayscale.bmp"))
        printf("ERROR: %s", SDL_GetError());
    SDL_FreeSurface(s);
    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(t);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return EXIT_SUCCESS;
}
