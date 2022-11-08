#include "use.h"

void to_case(SDL_Surface* surface, int list_coor[],  SDL_Renderer* renderer)
{

    int x = list_coor[2] - list_coor[0];
    int y = list_coor[5] - list_coor[1];

    SDL_Surface* new_surface = SDL_CreateRGBSurface(0, x, y, 32, 0, 0, 0, 0);

    SDL_Rect surfacerect;
    SDL_Rect newsurfacerect;

    surfacerect.x = list_coor[0];
    surfacerect.y = list_coor[1];
    surfacerect.w = x;
    surfacerect.h = y;

    newsurfacerect.x = 0;
    newsurfacerect.y = 0;
    newsurfacerect.w = x;
    newsurfacerect.h = y;

    SDL_BlitSurface(surface, &surfacerect, new_surface, &newsurfacerect);
    //copier surface sur une autre

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    //transformer surface to texture

    SDL_FreeSurface(new_surface);

}



// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* temp = IMG_Load(path);
    if (temp == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* def = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGB888,0);
    if (def == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_FreeSurface(temp);
    return def;
}


int main(int argc, char** argv)
{
    //argv[1] : nom de l'image
    //argv[2] : fichier contenant les coordonnees
    if(argc != 2)
    {
        errx(EXIT_FAILURE, "Usage: image-file");
    }

    int list_coor[] = {0,0,200,0,0,100}; //TODOOOOO



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

    // - Resize the window according to the size of the image.
    int w = surface->w;
    int h = surface->h;
    SDL_SetWindowSize(window, w, h);


    to_case(surface, list_coor, renderer);


    // - Free the surface.
    SDL_FreeSurface(surface);

    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
