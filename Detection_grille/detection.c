#include "../Traitement/use.h"
#include <math.h>


void foreach_pixels(Uint32 pixel_color, SDL_PixelFormat* format,
        SDL_Renderer* renderer, int x, int y)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    if (r != 0 && b != 0 && g != 0)
    {
        for(float t = 0; t < 180; t++)
        {
            x = x * cos(t);
            y = y * sin(t);
            SDL_RenderDrawPoint(renderer,x,y);
        }
    }
}

void hough_function(SDL_Surface* surface,SDL_Renderer* renderer)
{
    // Take the weight and height of my pixels
    Uint32* pixels = surface->pixels;
    int w = surface->w;
    int h = surface->h;
    SDL_PixelFormat* format = surface->format;

    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Try if a pixel is important or not and if it is, continue in another function
    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            foreach_pixels(pixels[j*w + w],format,renderer,w,h);
        }
    }

    SDL_UnlockSurface(surface);
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




    // Creates a window.
    SDL_Window* window1 = SDL_CreateWindow("Plain Window", 0, 0,100, 100,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window1 == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer1 = SDL_CreateRenderer(window1, -1, SDL_RENDERER_ACCELERATED);
    if (renderer1 == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());





    // Create a surface and a texture from the image in argument
    SDL_Surface* s = load_image(argv[1]);
    SDL_Texture* t =  SDL_CreateTextureFromSurface(renderer, s);

    //Go to the fonction for each pixels
    hough_function(s,renderer1);

    if(SDL_SaveBMP(s, "test_grayscale.bmp"))
        printf("ERROR: %s", SDL_GetError());
    SDL_FreeSurface(s);

    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(t);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer1);
    SDL_DestroyWindow(window1);
    SDL_Quit();


    return EXIT_SUCCESS;
}
