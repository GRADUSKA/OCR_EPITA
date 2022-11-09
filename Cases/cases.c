#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


void to_case(SDL_Surface* surface, int list_coor[],  SDL_Renderer* renderer)
{
    SDL_LockSurface(surface);

    int x = list_coor[2] - list_coor[0];
    int y = list_coor[5] - list_coor[1];

    SDL_Surface* new_surface = SDL_CreateRGBSurface(0, x, y, 32, 0, 0, 0, 0);
    SDL_LockSurface(new_surface);


    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Uint32* new_pixels = new_surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    int k = 0;
    for(int i = list_coor[1]; i < list_coor[5]; i++)
    {
        int l = 0;
        for(int j = list_coor[0]; j < list_coor[2]; j++)
        {
            new_pixels[k*x+l] = pixels[i*x+j];
            l++;
        }
        k++;
    }

    SDL_SaveBMP(new_surface, "sadnessss.bmp");

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
    //argv[1] : nom de l'image
    //argv[2] : fichier contenant les coordonnees
    if(argc != 2)
    {
        errx(EXIT_FAILURE, "Usage: image-file");
    }

    int list_coor[] = {0,0,500,0,0,500}; //TODOOOOO



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

    to_case(surface, list_coor, renderer);

    // - Free the surface.
    SDL_FreeSurface(surface);

    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
