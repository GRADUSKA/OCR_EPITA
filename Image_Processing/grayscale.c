#include "use.h"


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

    SDL_Surface* s = load_image(argv[1]);
    SDL_Texture* t =  SDL_CreateTextureFromSurface(renderer, s);

    surface_to_grayscale(s);

    SDL_Texture* grayt = SDL_CreateTextureFromSurface(renderer, s);

    if(SDL_SaveBMP(s, "test_grayscale.bmp"))
        printf("ERROR: %s", SDL_GetError());
    SDL_FreeSurface(s);
    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(t);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(grayt);
    SDL_Quit();


    return EXIT_SUCCESS;
}
