#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "use.h"

// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);
}

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    Uint32 average = 0.3*r + 0.59*g + 0.11*b;
    
    r = average;
    g = average;
    b = average;

    
    Uint32 color = SDL_MapRGB(format, r, g, b);
    return color; 
}

void fill_box(SDL_Surface* s, int w, int h, int len)
{
    s
}

void fill_grid(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    SDL_PixelFormat* format = surface->format;
    
    if (SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    SDL_LockSurface(surface);
    int h = (surface->h)/9;
    int w = (surface->w)/9;
    for (int j = 1; j < 9;j++)
    {
        fill_box(surface,w*j,h*j,w);
    }
    for(int j = 1; j < 9; j++)
    {
        int h = (surface->h)/9;
        h *= j;
        for(int x = 0; x < surface->w; x++)
            pixels[h*(surface->w) + x] = SDL_MapRGB(surface->format,0,255,0);
    }

    for(int j = 1; j < 9; j++)
    {
        int w = (surface->w)/9;
        w *= j;
        for(int y = 0; y < surface->h; y++)
            pixels[(surface->w)*y + w] = SDL_MapRGB(surface->format,255,0,0);
    }

    SDL_UnlockSurface(surface);
}

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");
    SDL_Window* window = SDL_CreateWindow("Result", 0, 0, 100, 100,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* surface = load_image(argv[1]);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    fill_grid(surface);
    SDL_SaveBMP(surface,"Result_grid.bmp");
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
