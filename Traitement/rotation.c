#include "use.h"

int main(int argc, char **argv)
{
    if(argc != 3)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf( "SDL_Init failed: %s",SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Rendering to a texture!",0,0, 1000,1000,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if(win ==NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1,
            SDL_RENDERER_ACCELERATED);

    if(renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());


    //Put your own bmp image here
    SDL_Surface *bmpSurf = load_image(argv[1]);
    SDL_Texture *bmpTex = SDL_CreateTextureFromSurface(renderer, bmpSurf);
    SDL_SetWindowSize(win, bmpSurf -> w, bmpSurf -> h);
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
    size_t i = 0;
    int is_neg = 0;
    if(argv[2][0] != '-' && (argv[2][0] < '0' || argv[2][0] > '9'))
        errx(EXIT_FAILURE, "Put an valid angle ");

    if(argv[2][0] == '-')
    {
        is_neg = 1;
        i++;
    }

    for(; argv[2][i] != 0; i++)
    {
        if ('0' <= argv[2][i] && argv[2][i] <= '9')
            angle = angle *10 + (argv[2][i] - '0');
    }

    if(is_neg)
    {
        angle = (360 - angle);
        while(angle < 0)
        {
            angle +=360;
        }
    }
    else
    {
        while(angle > 360)
        {
            angle -= 360;
        }
    }

    const double a = angle;

    SDL_RenderCopyEx(renderer, texTarget, NULL, NULL, a, NULL, SDL_FLIP_NONE);

    SDL_RenderPresent(renderer);
    SDL_Delay(20000);
    SaveScreenshot(renderer);


    SDL_DestroyTexture(texTarget);
    SDL_DestroyTexture(bmpTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
