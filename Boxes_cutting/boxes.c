#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Trained_Neurons/neural_network.h"
#include "../Trained_Neurons/neuron_utils.h"


SDL_Surface* to_case(SDL_Surface* surface, int x, int y, int w, int h)
{
    // to change : the width and the heigh by the real
    SDL_Surface* new_surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);

    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Uint32* new_pixels = new_surface->pixels;
    if (new_pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    SDL_LockSurface(surface);
    SDL_LockSurface(new_surface);

    //position "x" of the first pixel of the case in the grid
    int k = x;

    for(int i = 0; i < new_surface->h; i++)
    {
        // position "y" of the first pixel of the case in the grid
        int l = y;

        for(int j = 0; j < new_surface->w; j++)
        {
            new_pixels[i*new_surface->w+j] = pixels[k*surface->w+l];
            l++;
        }
        k++;
    }


    SDL_UnlockSurface(new_surface);

    SDL_UnlockSurface(surface);

    return new_surface;
}


SDL_Surface* load_image1(const char* path)
{
    SDL_Surface* tmp = IMG_Load(path);

    if(tmp == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface *res = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);

    if(res == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_FreeSurface(tmp);

    return res;
}


int boxes(char* path)
{
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Plain Window", 0, 0, 0, 0,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface.
    SDL_Surface* surface = load_image1(path);

    // - Create a texture from the image.
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int w = (surface->w)/9;
    int h = (surface->h)/9;
    int x = 0;
    int y = 0;

    size_t *sizes = malloc(sizeof(size_t) * 4);
    sizes[0] = (size_t) 256;
    sizes[1] = (size_t) 16;
    sizes[2] = (size_t) 16;
    sizes[3] = (size_t) 9;

    layers **layer_list = malloc(sizeof(layers*) * 4);
    init_layers(layer_list, sizes);
    matrix **W = malloc(sizeof(matrix*) * 3);
    init_weights(W, sizes);

    FILE* neuron_file = fopen("digits.txt", "r");
    if(neuron_file)
    {
        read_neuron(neuron_file, layer_list, W);
        fclose(neuron_file);
    }

    FILE *fp = fopen("grid", "w");
    for(size_t i = 0; i < 9; i++)
    {
       if(i != 0 && i % 3 == 0)
       {
           fputc('\n', fp);
       }
        for(size_t j = 0; j < 9; j++)
        {
            if(j != 0 && j % 3 == 0)
            {
                fputc(' ', fp);
            }
            SDL_Surface* surface = to_case(surface, x, y, w, h);
            int t = test11(surface);
            if(t > 10)
            {
                size_t num = neurons(surface, layer_list, W);
                fputc(num + '0',fp);
            }
            else
            {
                fputc('.',fp);
            }
            x += w;
        }
        y += h;
        fputc('\n', fp);

    }

    // - Free the surface.
    SDL_FreeSurface(surface);

    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
