#include "use.h"
#include <string.h>
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

void save_image(char* s, SDL_Renderer* renderer, SDL_Window* win)
{
    // Create an empty RGB surface that will be used to create the screenshot bmp file
    SDL_Surface* pScreenShot = SDL_CreateRGBSurface(0, 100, 100, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    if(pScreenShot)
    {
        // Read the pixels from the current render target and save them onto the surface
        SDL_RenderReadPixels(renderer, NULL, SDL_GetWindowPixelFormat(win), pScreenShot->pixels, pScreenShot->pitch);

        if(s == NULL)
            SDL_SaveBMP(pScreenShot, "Screenshot.bmp");

        else
            SDL_SaveBMP(pScreenShot, strcat(s, ".bmp\0"));

        // Destroy the screenshot surface
        SDL_FreeSurface(pScreenShot);

    }
}

void SaveScreenshot(SDL_Renderer *renderer)
{
    int w = 1920;
    int h = 1080;
    SDL_Surface *sshot = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    SDL_SaveBMP(sshot, "screenshot.bmp");
    SDL_FreeSurface(sshot);
}
