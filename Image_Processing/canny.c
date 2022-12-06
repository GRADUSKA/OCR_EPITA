#include "canny.h"
#include <stdio.h>
#define highratio 0.42
#define lowratio 0.34
#define KX (int[]){-1, 0, 1,-2, 0, 2,-1, 0, 1}
#define KY (int[]){1, 2, 1,0, 0, 0,-1, -2, -1}

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
    int* pixelsX = Convolution(surface, KX);
    int* pixelsY = Convolution(surface, KY);

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

double* direction_grad(int* Gx, int* Gy, int size)
{
    double* theta = malloc(sizeof(double) * size);
    for(int i =0; i< size;i++)
    {
        theta[i] = atan2(Gy[i],Gx[i]);
    }
    return theta;
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

double* non_max_suppr(SDL_Surface *surface, double* theta)
{
    int size = surface->w * surface->h;
    Uint32* pixels = surface->pixels;
    double* res = malloc(sizeof(double) * size);
    for(int i = 0 ; i < size;i++)
    {
        theta[i] = theta[i]*180. / M_PI;
        if(theta[i] < 0)
            theta[i]+=180;
    }

    for(int i = 0; i < surface->h; i++)
    {
        for(int j = 0;j < surface->w;j++)
        {
            int q = 255;
            int r = 255;

            int cell = i*surface->w+j;

            if((0 <= theta[cell] && theta[cell] < 22.5) || (157.5 <= theta[cell]
                        && theta[cell] <= 180))
            {
                q = pixels[cell+1];
                r = pixels[cell-1];
            }


            else if(22.5 <= theta[cell] && theta[cell] < 67.5)
            {
                q = pixels[(i+1)*surface->w +j-1];
                r = pixels[(i-1)*surface->w +j+1];
            }

            else if(67.5 <= theta[cell] && theta[cell] < 112.5)
            {
                q = pixels[(i+1)*surface->w +j];
                r = pixels[(i-1)*surface->w +j];
            }

            else if(112.5 <= theta[cell] && theta[cell] < 157.5)
            {
                q = pixels[(i-1)*surface->w + j - 1];
                r = pixels[(i+1)*surface->w + j + 1];
            }

            if((int)pixels[cell] >= q && (int)pixels[cell] >= r)
                res[cell] = (double)pixels[cell];
            else
                res[cell] = 0;
        }
    }
    return res;
}

char* double_threshold(SDL_Surface* s, double Low_ratio, double High_Ratio)
{
    int w = s->w;
    int h = s->h;
    int size = w*h;
    double max = 0;
    Uint32* pixels = s-> pixels;

    for(int i = 0; i < size;i++)
    {
        if(pixels[i] > max)
            max = pixels[i];
    }

    double Highthreshold = max * High_Ratio;
    double Lowthreshold = Low_ratio * Highthreshold;

    char* res = malloc(sizeof(char) * size);
    for(int i = 0; i < size; i++)
    {
        if(pixels[i] > Highthreshold)
            res[i] = 2;
        else if(pixels[i] < Lowthreshold)
            res[i] = 0;
        else
            res[i] = 1;
    }

    return res;
}

void hyperthesis(SDL_Surface* s)
{
    int w = s->w;
    int h = s->h;

    char* thresholds = double_threshold(s,lowratio,highratio);

    Uint32* pixels = s->pixels;
    for(int i = 0 ; i < h;i++)
    {
        for(int j = 0; j < w; j++)
        {
            if(thresholds[i*w+j] == 1)
            {
                for(int k = 0; k < 3;k++)
                {
                    for(int l = 0; l< 3;l++)
                    {
                        int _i_ = i + k - 1;
                        int _j_ = j + l - 1;
                        if(_i_ < 0)
                            _i_ =0;
                        if (_i_ >= s->h)
                            _i_ = s->h-1;
                        if(_j_ < 0)
                            _j_ =0;
                        if (_j_ >= s->w)
                            _j_ = s->w-1;
                        if(thresholds[_i_ * l + _j_] == 2)
                            thresholds[i*w +j] = 2;
                    }
                }
                if(thresholds[i*w+j] != 2)
                    thresholds[i*w+j] = 0;
                pixels[i*w+j] = (thresholds[i*w+j]/2) * 255;
            }
        }
    }
}
