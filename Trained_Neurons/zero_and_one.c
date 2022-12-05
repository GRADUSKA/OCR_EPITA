#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "neuron_utils.h"


int which(int c, int surf, int max, Uint32* pixels, SDL_PixelFormat* format, int x, int y)
{
    int noir = 0;
    int blanc = 0;
    for(int p = 0; p < y; p++)
    {
        for(int q = 0; q < x; q++)
        {
            Uint8 r, g, b;
            if(c+p+surf*q < max)
            {
                SDL_GetRGB(pixels[c+p+surf*q], format, &r, &g, &b);
                if(r >= 128 && g >= 128 && b >= 128)
                {
                    blanc++;
                }
                else
                {
                    noir++;
                }
            
            }
        }
    }

    if(noir > blanc)
    {
        return 1;
    }
    return 0;
}

void zeroandone(SDL_Surface* surface, layers* layer)
{
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }
    int base_x = 0;
    int base_y = 0;
    int max_x = surface->w;
    int max_y = surface->h;
    if(max_x % 2)
        max_x++;
    if(max_y % 2)
        max_y++;
    for(int alt = 0; (max_x - base_x) % 16; alt = !alt)
    {
        if(alt)
            max_x += 2;
        else
            base_x -= 2;
    }
    for(int alt = 0; (max_y - base_y) % 16; alt = !alt)
    {
        if(alt)
            max_y += 2;
        else
            base_y -= 2;
    }
    int width = max_x - base_x;
    int height = max_y - base_y;

    int x = width/16;
    int y = height/16;

    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);

    size_t h = 0;
    for(int i = base_y; h < 16; i+=y, h++)
    {
        size_t w = 0;
        for(int j = base_x; w < 16; j+=x, w++)
        {
             int n = which(i*width+j, width, width*height, pixels, format, x, y);
             layer->neurons[h * x + w] = n;
        }

    }
    SDL_UnlockSurface(surface);
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* temp = IMG_Load(path);
    if (temp == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* def = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGB888,0);
    if (def == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_FreeSurface(temp);
    return def;
}

void transform(const char* path, layers* layer)
{
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Plain Window", 0, 0, 0, 0,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the colored image.
    SDL_Surface* surface = load_image(path);

    // - Create a texture from the image.
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    zeroandone(surface, layer);

    // - Free the surface.
    SDL_FreeSurface(surface);

    // Destroys the objects.
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
