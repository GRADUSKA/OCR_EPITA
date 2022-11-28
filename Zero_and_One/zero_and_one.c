#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int which(int c, int surf, int max, Uint32* pixels, SDL_PixelFormat* format, int x, int y)
{
    int noir = 0;
    int blanc = 0;
    for(int p = 0; p < y; p++)
    {
        for(int q = 0; q < x; q++)
        {
            Uint8 r, g, b;
            if(c+p+surf*q < max)
            {
                SDL_GetRGB(pixels[c+p+surf*q], format, &r, &g, &b);
                if(r == 255 && g == 255 && b == 255)
                {
                    blanc++;
                }
                else
                {
                    noir++;
                }
            
            }
        }
    }

    if(noir > blanc)
    {
        return 1;
    }
    return 0;
}

void zeroandone(SDL_Surface* surface)
{
    FILE *file = fopen("zero_and_one.txt","w");

    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    int x = (surface->w)/16;
    int y = (surface->h)/16;

    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);

    for(int i = 0; i < surface->h; i+=y)
    {
        for(int j = 0; j < surface->w; j+=x)
        {
             int n = which(i*surface->w+j, surface->w, surface->w*surface->h, pixels, format, x, y);
             if(n == 1)
             {
                 fputc('1', file);
             }
             else
             {
                 fputc('0', file);
             }
        }
        fputc('\n', file);

    }
    fputc('\0', file);
    SDL_UnlockSurface(surface);
    fclose(file);
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
    if(argc != 2)
    {
        errx(EXIT_FAILURE, "Usage: image-file num");
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

    // - Create a surface from the colored image.
    SDL_Surface* surface = load_image(argv[1]);

    // - Create a texture from the image.
    SDL_Texture* texture = IMG_LoadTexture(renderer, argv[1]);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    zeroandone(surface);

    // - Free the surface.
    SDL_FreeSurface(surface);

    // Destroys the objects.
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}
