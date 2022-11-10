#include <stdio.h>
#include <math.h>
#include "use.h"

double GaussianBlur[] = {1./256., 4./256.,  6./256.,  4./256.,  1./256.,
    4./256., 16./256., 24./256., 16./256., 4./256.,
    6./256., 24./256., 36./256., 24./256., 6./256.,
    4./256., 16./256., 24./256., 16./256., 4./256.,
    1./256., 4./256.,  6./256.,  4./256.,  1./256.};

Uint32* Convolution(SDL_Surface* surface)
{
    Uint32* pixels = surface -> pixels;
    Uint32* result = malloc(surface->h * surface->w * sizeof(Uint32*));

    SDL_LockSurface(surface);
    for(int i = 0; i < surface->h;i++)
    {
        for(int j = 0; j < surface->w;j++)
        {
            double resR = 0, resG = 0, resB = 0;
            for(int k = 0; k < 5;k++)
            {
                for(int l = 0; l< 5;l++)
                {
                    int _i_ = i + k -2;
                    int _j_ = j + l -2;
                    if(_i_ < 0)
                        _i_ =0;
                    if (_i_ >= surface->h)
                        _i_ = surface->h-1;
                    if(_j_ < 0)
                        _j_ =0;
                    if (_j_ >= surface->w)
                        _j_ = surface->w-1;
                    Uint8 r,g,b;
                    SDL_GetRGB(pixels[_i_ * surface->w + _j_], surface->format
                            , &r,&g,&b);
                    resR += r * GaussianBlur[k*5+l];
                    resG += g * GaussianBlur[k*5+l];
                    resB += b * GaussianBlur[k*5+l];
                }
            }
            result[i* surface->w+j] = SDL_MapRGB(surface->format, (int)resR%256,
                    (int)resG%256, (int)resB%256);
        }
    }
    SDL_UnlockSurface(surface);
    return result;
}

void ApplyGaussian(SDL_Surface *surface)
{
    Uint32* pixels = Convolution(surface);
    int size = surface->w * surface->h;
    Uint32* res = surface->pixels;
    SDL_LockSurface(surface);

    for(int i = 0; i < size;i++)
    {
        res[i] = pixels[i];
    }
    SDL_UnlockSurface(surface);
    free(pixels);
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    SDL_Surface* s = load_image(argv[1]);

    ApplyGaussian(s);

    SDL_SaveBMP(s, "test_gaussian_blur.bmp");

    SDL_FreeSurface(s);

    // Destroys the objects.
    SDL_Quit();


    return EXIT_SUCCESS;
}
