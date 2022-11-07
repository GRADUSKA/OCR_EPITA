#include <math.h>
#include "use.h"

int Kx[] = {
    -1, 0, 1,
    -2, 0, 2,
    -1, 0, 1
};

int Ky[] = {
    1, 2, 1,
    0, 0, 0,
    -1, -2, -1
};

int* Convolution(SDL_Surface* surface, int filtre[])
{
    Uint32* pixels = surface -> pixels;
    int* result = malloc(surface->h * surface->w * sizeof(int));

    SDL_LockSurface(surface);
    for(int i = 0; i < surface->h;i++)
    {
        for(int j = 0; j < surface->w;j++)
        {
            double resR = 0, resG = 0, resB = 0;
            for(int k = 0; k < 3;k++)
            {
                for(int l = 0; l< 3;l++)
                {
                    int _i_ = i + k - 1;
                    int _j_ = j + l - 1;
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
                    resR += r * filtre[k*3+l];
                    resG += g * filtre[k*3+l];
                    resB += b * filtre[k*3+l];
                }
            }
            result[i* surface->w+j] = resR;
        }
    }
    SDL_UnlockSurface(surface);
    return result;
}

double* gradiants(SDL_Surface *surface)
{
    int* pixelsX = Convolution(surface, Kx);
    int* pixelsY = Convolution(surface, Ky);

    int size = surface->w * surface->h;

    double* res = malloc(sizeof(double) * size);
    double max = 0;
    for(int i = 0; i < size;i++)
    {
        double nb = sqrt((double)pixelsX[i] * (double)pixelsX[i] +
                (double)pixelsY[i] * (double)pixelsY[i]);
        res[i] = nb;
        if (nb > max)
            max = nb;
    }
    for(int i = 0; i < size;i++)
    {
        res[i] = res[i]/max * 255;
        res[i]= res[i] > 255 ? 255 : res[i];
        res[i] = res[i] < 0 ? 0 : res[i];
    }
    free(pixelsX);
    free(pixelsY);
    return res;
}
void ApplySobel(SDL_Surface *surface)
{
    double* graduska= gradiants(surface);
    int size = surface->w * surface->h;
    Uint32* res = surface->pixels;
    SDL_LockSurface(surface);

    for(int i = 0; i < size;i++)
    {
        res[i] = SDL_MapRGB(surface -> format,graduska[i],graduska[i],graduska[i]);
    }
    SDL_UnlockSurface(surface);
    free(graduska);
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    SDL_Surface* s = load_image(argv[1]);

    ApplySobel(s);

    SDL_SaveBMP(s, "test_Sobel_blur.bmp");

    SDL_FreeSurface(s);

    // Destroys the objects.
    SDL_Quit();


    return EXIT_SUCCESS;
}
