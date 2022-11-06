#include "rotation.h"


void draw(SDL_Renderer* renderer)
{
    SDL_RenderPresent(renderer);
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


void event_loop(SDL_Renderer* renderer )
{
    SDL_Event event;

    while (1)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                return;

            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_RESIZED)
                    draw(renderer);
                break;

        }
    }
}





int main(int argc, char **argv)
{
    if(argc != 3)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf( "SDL_Init failed: %s",SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Rendering to a texture!",0,0, 1920,1080,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if(win ==NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    if(renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    //Put your own bmp image here
    SDL_Surface *bmpSurf = load_image(argv[1]);
    SDL_Texture *bmpTex = SDL_CreateTextureFromSurface(renderer, bmpSurf);
    SDL_FreeSurface(bmpSurf);

    //Make a target texture to render too
    SDL_Texture *texTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, 1920, 1080);

    //Now render to the texture
    SDL_SetRenderTarget(renderer, texTarget);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bmpTex, NULL, NULL);

    //Detach the texture
    SDL_SetRenderTarget(renderer, NULL);

    //Now render the texture target to our screen, but upside down
    SDL_RenderClear(renderer);

    double angle = 0;
    for(size_t i = 0; argv[2][i] != 0; i++)
    {
        if ('0' <= argv[2][i] && argv[2][i] <= '9')
            angle = angle *10 + (argv[2][i] - '0');
    }

    const double a = angle;

    SDL_RenderCopyEx(renderer, texTarget, NULL, NULL, a, NULL, SDL_FLIP_NONE);


    event_loop(renderer);


    SDL_DestroyTexture(texTarget);
    SDL_DestroyTexture(bmpTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
