#include "use.h"
#include <stdio.h>
#include <math.h>

//Deux problemes possibles :
//1 : je remplis mal mon array
//2 : j'affiche mal mes lignes


void foreach_pixels(Uint32 pixel_color, SDL_PixelFormat* format,
        int* tab, float x, float y,int height)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    if (r > 0 && b > 0 && g > 0)
    {
        for(int t = 0; t < 360; t++)
        {
            double radian = t*M_PI/180;
            int rho = x * cos(radian) + y * sin(radian);
            tab[(rho + (height/2)) + t * 360]+=1;
        }
    }
}

int* hough_function(SDL_Surface* surface, int* w, int* h)
{
    // Take the weight and height of my pixels
    Uint32* pixels = surface->pixels;
    *w = surface->w;
    *h = surface->h;


    //Create the matrice
    int width = 360;
    int height = 2 * sqrt((*w)*(*w) + (*h)*(*h));
    int* tab = calloc((width * height),sizeof(int));
    SDL_PixelFormat* format = surface->format;

    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_LockSurface(surface);

    // Try if a pixel is important or not and if it is, continue in another function
    for(int y = 0; y < (*h); y++)
    {
        for(int x = 0; x < (*w); x++)
        {
            foreach_pixels(pixels[(*w)*y + x],format,tab,(float)x,(float)y, height);
        }
    }

    SDL_UnlockSurface(surface);
    return tab;
}


//a enlever
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
void drawHoughSpace(SDL_Surface* s,int w,int h,int* array)
{
    int w_s = s->w;
    int h_s = s->h;

    Uint32* pixels = s->pixels;

    int max = 0;
    for(int i = 0; i < (2 * 360 * sqrt((w_s*w_s) + (h_s*h_s)));i++)
    {
        if (max < array[i])
            max = array[i];
    }
 
    for(int y_tab = 0; y_tab < h; y_tab++)
    {
        for(int x_tab = 0; x_tab < w; x_tab++)
        {
            if((array[y_tab * w + x_tab]) / ((70 * max)/100) != 0)
            {
                double rad = x_tab* M_PI / 180;
                if((x_tab > 45 && x_tab <= 135) || (x_tab > 225 && x_tab <= 315))
                {
                    for(int x = 0; x < w_s; x++)
                    {
                        int y = ((y_tab - h/2) - x*cos(rad))/sin(rad);
                        if (y < h_s && y > 0)
                            pixels[y * w_s + x] = SDL_MapRGB(s->format,0,255,0);
                    }
                }
                else
                {
                    for(int y = 0; y < h_s; y++)
                    {
                        int x = ((y_tab - h/2) - y * sin(rad)) / cos(rad);
                        if(x < w_s && x > 0)
                            pixels[y * w_s + x] = SDL_MapRGB(s->format,0,255,0);
                    }
                }
            }
        }
    }
}


int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");
    SDL_Surface* s = load_image(argv[1]);

    //Call the hough function
    int w = 0;
    int h = 0;
    int* tab = hough_function(s,&w,&h);
    drawHoughSpace(s,w,h,tab);
    SDL_SaveBMP(s,"merde.bmp");

    // Destroys the objects.
    SDL_FreeSurface(s);
    SDL_Quit();


    return EXIT_SUCCESS;
}
