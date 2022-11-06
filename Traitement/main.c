#include "rotation.h"

int main(int argc, char** argv)
{
    if(argc > 2)
        errx(EXIT_FAILURE, "Too much arguments");
    if(argc <= 1)
        errx(EXIT_FAILURE, "Missing one argument");
    else
    {
        SDL_Surface *surface = IMG_Load(argv[1]);
        SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,surface);
        rot_right(renderer,texture,90);
    }
    return 0;
}
