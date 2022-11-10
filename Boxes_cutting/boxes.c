#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


void to_case(SDL_Surface* surface)
{   

    SDL_Surface* new_surface = SDL_CreateRGBSurface(0, (surface->w)/9, (surface->h)/9, 32, 0, 0, 0, 0); 
    // to change : the width and the heigh by the real 

    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Uint32* new_pixels = new_surface->pixels;
    if (new_pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    SDL_LockSurface(surface);
    SDL_LockSurface(new_surface);

    int k = (surface->w)/9; 
    //position "x" of the first pixel of the case in the grid
    
    for(int i = 0; i < new_surface->h; i++)
    {
        int l = 0; 
        // position "y" of the first pixel of the case in the grid
        
        for(int j = 0; j < new_surface->w; j++)
        {
            new_pixels[i*new_surface->w+j] = pixels[k*surface->w+l];
            l++;
        }
        k++;
    }

    SDL_SaveBMP(new_surface, "boxe.bmp");

    SDL_UnlockSurface(new_surface);

    SDL_FreeSurface(new_surface);

    SDL_UnlockSurface(surface);

}


SDL_Surface* load_image(const char* path)
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


int main(int argc, char** argv)
{
    //argv[1] : name of the image
    //argv[2] : file with the coordonates
    if(argc != 2)
    {
        errx(EXIT_FAILURE, "Usage: image-file");
    }

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
    SDL_Surface* surface = load_image(argv[1]);

    // - Create a texture from the image.
    SDL_Texture* texture = IMG_LoadTexture(renderer, argv[1]);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    to_case(surface);

    // - Free the surface.
    SDL_FreeSurface(surface);

    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
