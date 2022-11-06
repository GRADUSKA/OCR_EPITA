#include "use.h"

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
