#include "use.h"
#include "../Image_Processing/rotation.h"
#include "blob.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
typedef struct vector
{
    size_t capacity;
    size_t size;
    int *data;
}vector;

struct vector *vector_new()
{
    struct vector *viktor = malloc(sizeof(struct vector));
    if(!viktor)
        errx(1,"Not enough memory!");
    viktor->capacity = 1;
    viktor->size = 0;
    viktor->data = malloc(sizeof(int));
    if(!viktor->data)
        errx(1,"Not enough memory!");
    return viktor;
}

void vector_free(struct vector *v)
{
    free(v->data);
    free(v);
}

void double_capacity(struct vector *v)
{
    v->capacity *= 2;
    v->data = realloc(v->data, v->capacity * sizeof(int));
    if(!v->data)
        errx(1,"Not enough memory!");
}

void vector_push(struct vector *v, int x)
{
    if(v->size+1 > v->capacity)
        double_capacity(v);
    v->data[v->size] = x;
    v->size++;
}

//Deux problemes possibles :
//1 : je remplis mal mon array
//2 : j'affiche mal mes lignes


void foreach_pixels(int* tab, float x, float y,int height)
{
    for(int t = 0; t < 360; t++)
    {
        double radian = t*M_PI/180;
        int rho = x * cos(radian) + y * sin(radian);
        tab[(rho + height/2)*360 + t]+=1;
    }
}

int* hough_function(SDL_Surface* surface)
{
    // Take the weight and height of my pixels
    Uint32* pixels = surface->pixels;
    int w = surface->w;
    int h = surface->h;


    //Create the matrice
    int width = 360;
    int height = 2 * sqrt((w*w) + (h*h));
    int* tab = calloc((width * height),sizeof(int));

    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_LockSurface(surface);

    // Try if a pixel is important or not and if it is, continue in another function
    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            if (pixels[x + y*w] != 0)
                foreach_pixels(tab,x,y, height);
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
void drawHoughSpace(SDL_Surface* s,int* array)
{
    int w_s = s->w;
    int h_s = s->h;
    int w = 360;
    int h = sqrt((w_s * w_s) + (h_s * h_s))*2;

    Uint32* pixels = s->pixels;

    int max = array[0];
    for(int i = 0; i < (w*h);i++)
    {
        if (max < array[i])
            max = array[i];
    }

    for(int y_tab = 0; y_tab < h; y_tab++)
    {
        for(int x_tab = 0; x_tab < w; x_tab++)
        {
            if((array[y_tab * w + x_tab]) / ((80 * max)/100) != 0)
            {
                double rad = x_tab* M_PI / 180;
                if((x_tab > 45 && x_tab <= 135) || (x_tab > 225 && x_tab <= 315))
                {
                    for(int x = 0; x < w_s; x++)
                    {
                        int y = ((y_tab - h/2) - x*cos(rad)) / sin(rad);
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


double get_angles(SDL_Surface* s,int* array)
{
    int w_s = s->w;
    int h_s = s->h;
    int w = 360;
    int h = sqrt((w_s * w_s) + (h_s * h_s))*2;

    vector *anglesX = vector_new(); 
    vector *anglesY = vector_new();

    int max = array[0];
    for(int i = 0; i < (w*h);i++)
    {
        if (max < array[i])
            max = array[i];
    }

    for(int y_tab = 0; y_tab < h; y_tab++)
    {
        for(int x_tab = 0; x_tab < w; x_tab++)
        {
            if((array[y_tab * w + x_tab]) / ((80 * max)/100) != 0)
            {
                if((x_tab > 45 && x_tab <= 135) || (x_tab > 225 && x_tab <= 315))
                    vector_push(anglesX,x_tab);
                else
                    vector_push(anglesY,x_tab);                   
            }
        }
    }

    int res = 0;
    if(anglesX->size > anglesY->size)
    {
        for(size_t i = 0; i < anglesX->size;i++)
            res += anglesX->data[i];
        return res/anglesX->size;
    }
    else    
    {
        for(size_t i = 0; i < anglesY->size;i++)
            res += anglesY->data[i];
        return res/anglesY->size;
    }
}
int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: image-file");
    SDL_Surface* s = load_image(argv[1]);

    if(s == NULL)
        errx(EXIT_FAILURE,"NOT A GOOD IMAGE");
    char* a = argv[2];
    if(strcmp(a,"--hough") == 0)
    {
        int* tab = hough_function(s);
        drawHoughSpace(s,tab);
        SDL_SaveBMP(s,"merde.bmp");
        double angle = get_angles(s,tab);
        if(angle > 0)
        {
            Rotation_shearing(s,angle);
        }
    }
    else if (strcmp(a,"--blob")==0)
    {
        printf("a mimir\n");
        apply_blob_crop(s);
        printf("heu bahe jsp\n");
    }
    else
    {
        errx(EXIT_FAILURE,"NOT A GOOD ARGUMENT");
    }

    // Destroys the objects.
    SDL_Quit();


    return EXIT_SUCCESS;
}
