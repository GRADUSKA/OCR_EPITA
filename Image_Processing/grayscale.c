#include "grayscale.h"

Uint8 f(Uint8 c, double n)
{
    if(c <= 255/2)
        return (Uint8)((255/2)*SDL_pow((double)2*c/255,n));
    return 255 - f(255-c,n);
}


Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;

    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    Uint32 average = 0.3*r + 0.59*g + 0.11*b;

    r = average;
    g = average;
    b = average;
    r = f(r,10);
    g = f(g,10);
    b = f(b,10);

    return SDL_MapRGB(format, r, g, b);
}

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;

    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for(int i = 0; i < len; i++)
    {
        pixels[i] = pixel_to_grayscale(pixels[i], format);
    }

    SDL_UnlockSurface(surface);
}


