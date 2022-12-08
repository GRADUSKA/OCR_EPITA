#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "use.h"

int test(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    if(r < 125 && g < 125 && b < 125)
        return 0;
    return 1;
}

void fill_box(SDL_Surface* s, int w, int h, int len,Uint32* pix,int color,int number)
{

    SDL_Surface* surface = load_image("Source/8.jpg");

    if(number == 1)
    {
        surface = load_image("Source/1.jpg");
    }
    else if(number == 2)
    {
        surface = load_image("Source/2.jpg");
    }
    else if(number == 3)
    {
        surface = load_image("Source/3.jpg");
    }
    else if(number == 4)
    {
        surface = load_image("Source/4.jpg");
    }
    else if(number == 5)
    {
        surface = load_image("Source/5.jpg");
    }
    else if(number == 6)
    {
        surface = load_image("Source/6.jpg");
    }
    else if(number == 7)
    {
        surface = load_image("Source/7.jpg");
    }

    Uint32* pixels_number = surface->pixels;
    SDL_PixelFormat* format = surface->format;
    for(int i = 0; i < surface->h;i++)
    {
        for(int j = 0; j < surface->w; j++)
        {
            if (test(pixels_number[i*(surface->w) + j],format) == 0)
                pix[(h+i)*(s->w) + (w+j)] = SDL_MapRGB(surface->format,color,0,0);
        }
    }
    SDL_FreeSurface(surface);
}

void fill_grid(SDL_Surface* surface,int* tab,int* origin)
{
    Uint32* pixels = surface->pixels;

    if (SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_LockSurface(surface);
    int h = (surface->h)/9;
    int w = (surface->w)/9;
    int color = 0;
    for (int j = 0; j < 9;j++)
    {
        for(int u = 0; u < 9; u++)
        {
            if(tab[j*9+u] != origin[j*9+u])
                color = 255;
            else
                color=0;
            fill_box(surface,w*u,h*j,w,pixels,color,tab[j*9+u]);
        }
    }
    SDL_UnlockSurface(surface);
}

int* upload(char* f_name)
{
    int* tab = calloc(81,(sizeof(int)));
    FILE* fichier;
    int u = 0;
    fichier = fopen(f_name,"r");
    for(int i = 0; i < 109; i++)
    {
        char c = fgetc(fichier);
        int j = c-'0';
        if (j >= 0 && j <= 9)
        {
            tab[u] = j;
            u++;
        }
        else if(c == '.')
        {
            tab[u] = 0;
            u++;
        }
    }
    return tab;
}


int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");
    SDL_Surface* surface = load_image(argv[1]);
    int* tab = upload("grid.result");
    int* tab2 = upload("grid");
    fill_grid(surface,tab,tab2);
    SDL_SaveBMP(surface,"Result_grid.bmp");
    SDL_FreeSurface(surface);
    SDL_Quit();

    return EXIT_SUCCESS;
}
