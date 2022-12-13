#include "detection.h"
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

void vector_sort(vector* v)
{
    for(size_t i =0; i < v->size;i++)
    {
        for(size_t j =i; j < v->size-1;j++)
        {
            if(v->data[j] > v->data[j+1])
            {
                int mem = v->data[j];
                v->data[j] = v->data[j+1];
                v->data[j+1] = mem;
            }
        }
    }
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


int get_angles(SDL_Surface* s,int* array)
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
                {
                    vector_push(anglesX,x_tab);
                }
                else
                {
                    vector_push(anglesY,x_tab);

                }
            }
        }
    }

    if(anglesX->size > anglesY->size)
    {
        vector_sort(anglesX);
        size_t sss = anglesY->size;
        return 90 - anglesX->data[sss/2];
    }
    else    
    {
        vector_sort(anglesY);
        size_t sss = anglesY->size;
        return anglesY->data[sss/2] * -1;
    }

}
int detection(char* path,char* ar,char* angle)
{
    // Checks the number of arguments.
    SDL_Surface* s = load_image(path);

    if(s == NULL)
        errx(EXIT_FAILURE,"NOT A GOOD IMAGE");
    if(strcmp(ar,"--rotation")==0)
    {
        image_process(path,"--canny","0");
        double a = get_angle(angle);
        SDL_Surface* res = Rotation_shearing(s,a);
        SDL_SaveBMP(res,"rota.bmp");
    }
    else if(strcmp(ar,"--hough") == 0)
    {
        image_process(path,"--canny","0");
        SDL_Surface* surf = load_image("Canny.bmp");
        int* tab = hough_function(surf);
        drawHoughSpace(surf,tab);
        SDL_SaveBMP(surf,"Hough.bmp");
        int angle = get_angles(surf,tab);
        SDL_Surface* res = Rotation_shearing(s,angle);
        SDL_SaveBMP(res,"Rotation.bmp");
    }
    else if (strcmp(ar,"--blob")==0)
    {
        image_process(path,"--canny","0");
        SDL_Surface* canny = load_image("Canny.bmp");
        apply_blob_crop(canny);
    }
    else if(strcmp(ar,"--all") == 0)
    {
        image_process(path,"--all","0");
        SDL_Surface* surf = load_image("ImageProcessing.bmp");
        int* tab = hough_function(surf);
        drawHoughSpace(surf,tab);
        SDL_SaveBMP(surf,"Hough.bmp");
        int angle = get_angles(surf,tab);
        SDL_Surface* res = Rotation_shearing(load_image(path),angle);
        SDL_SaveBMP(res,"Rotation.bmp");
        image_process(path,"--canny","0");
        SDL_Surface* c = load_image("Canny.bmp");
        Blob_list* bloblist = generateBlob(c);
        Blob* final_blob = merge_blobs(bloblist);
        SDL_Surface* cropped = crop(c,final_blob);
        SDL_FreeSurface(s);
        SDL_SaveBMP(cropped,"Blob.bmp");
        SDL_FreeSurface(cropped);
    }
    else
    {
        errx(EXIT_FAILURE,"NOT A GOOD ARGUMENT");
    }

    // Destroys the objects.
    SDL_Quit();


    return EXIT_SUCCESS;
}
