#include "use.h"
#include <stdio.h>
#include <math.h>


void foreach_pixels(Uint32 pixel_color, SDL_PixelFormat* format,
        int* tab, int x, int y,int taille)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    if (r != 0 && b != 0 && g != 0)
    {
        for(int t = 0; t < 360; t++)
        {
            int r = x * cos((t * M_PI)/180) + y * sin((t* M_PI)/180);
            if(r >= 0 && r < taille)
            {
                tab[r * 360 + t]+=1;
            }
        }
    }
}

int* hough_function(SDL_Surface* surface, int* w_1, int* h_1)
{
    // Take the weight and height of my pixels
    Uint32* pixels = surface->pixels;
    int w  = surface->w;
    int h = surface->h;

    int taille = 360*sqrt((w*w)+(h*h));
    int* tab = calloc(taille,sizeof(int));
    SDL_PixelFormat* format = surface->format;

    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_LockSurface(surface);

    // Try if a pixel is important or not and if it is, continue in another function
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            foreach_pixels(pixels[w*i + j],format,tab,i,j, taille/360);
        }
    }

    SDL_UnlockSurface(surface);
    *w_1 = w;
    *h_1 = h;
    return tab;
}

SDL_Renderer* create_the_beautiful_function(int* tab, int* w_1, int* h_1)
{
    int max = 0;
    int w = *w_1;
    int h = *h_1;
    for(int i = 0; i < (360 * sqrt((w*w)+(h*h)));i++)
    {
        if (max < tab[i])
            max = tab[i];
    }
    // Creates a window.
    SDL_Window* window1 = SDL_CreateWindow("Plain Window", 0, 0,360, sqrt((w*w)+(h*h)),0);
    if (window1 == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // Creates a renderer.
    SDL_Renderer* renderer1 = SDL_CreateRenderer(window1, -1, SDL_RENDERER_ACCELERATED);
    if (renderer1 == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    //Clear renderer
    SDL_SetRenderDrawColor(renderer1, 0, 0, 0, 255);
    SDL_RenderClear(renderer1);

/*    SDL_Surface* new_surface = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);
    Uint32* pixels = new_surface->pixels;
    for (int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            if (tab[y*w+x] > 0)
            {
                int r = pixels[y*w+x] * 255 / max;
                pixels[y*w+x] = SDL_MapRGB(new_surface->format,r,r,r);
            }
        }
    }
    return new_surface;
*/
    //Do the lines
    SDL_SetRenderDrawColor(renderer1,255,255,255,255);
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            int color = (tab[i*w + j]*255)/max;
            SDL_SetRenderDrawColor(renderer1,color,color,color,color);
            SDL_RenderDrawPoint(renderer1,i,j);
        }
    }
    SDL_RenderPresent(renderer1);
    return renderer1;
}

void event_loop()
{
    // Creates a variable to get the events.
    SDL_Event event;

    while (1)
    {
        // Waits for an event.
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;
                
        }
    }
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("GRAY", 0, 0, 100, 100,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // Creates a renderer
    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Create a surface and a texture from the image in argument
    SDL_Surface* s = load_image(argv[1]);
    SDL_Texture* t =  SDL_CreateTextureFromSurface(renderer, s);


    //Call the hough function
    int w = 0;
    int h = 0;
    int* tab = hough_function(s,&w,&h);
    SDL_Renderer* r = create_the_beautiful_function(tab,&w,&h);
    event_loop();
    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyRenderer(r);
    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return EXIT_SUCCESS;
}
