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
                if(r >= 128 && g >= 128 && b >= 128)
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

void resize(SDL_Surface* surface, Uint32* pixels, SDL_PixelFormat* format, int* base_x, int* base_y, int* max_x, int* max_y)
{
    for(int y = 0; y < surface->h; y++)
    {
        for(int x = 0; x < surface->w; x++)
        {
            Uint8 r, g, b;
            SDL_GetRGB(pixels[x+surface->w*y], format, &r, &g, &b);
            if(r < 128 && g < 128 && b < 128)
            {
                if(*base_x > x)
                    *base_x = x;
                if(*max_x < x)
                    *max_x = x;
                if(*base_y > y)
                    *base_y = y;
                if(*max_y < y)
                    *max_y = y;
            }    
        }
    }
}

void zeroandone(SDL_Surface* surface)
{
    FILE *file = fopen("zero_and_one.txt","w");

    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    SDL_PixelFormat* format = surface->format;

    int base_x = surface->w;
    int base_y = surface->h;
    int max_x = 0;
    int max_y = 0;
    resize(surface, pixels, format, &base_x, &base_y, &max_x, &max_y);
    if(max_x % 2)
        max_x++;
    if(max_y % 2)
        max_y++;
    for(int alt = 0; (max_x - base_x) % 16; alt = !alt)
    {
        if(alt)
            max_x += 2;
        else
            base_x -= 2;
    }
    for(int alt = 0; (max_y - base_y) % 16; alt = !alt)
    {
        if(alt)
            max_y += 2;
        else
            base_y -= 2;
    }
    int width = max_x - base_x;
    int height = max_y - base_y;

    int x = width/16;
    int y = height/16;


    SDL_LockSurface(surface);

    size_t h = 0;
    for(int i = base_y; h < 16; i+=y, h++)
    {
        size_t w = 0;
        for(int j = base_x; w < 16; j+=x, w++)
        {
             if(j < 0 || i < 0 || j > surface->w || i > surface->h)
             {
                 fputc('0', file);
             }
             else
             { 
                 int n = which(i*width+j, width, width*height, pixels, format, x, y);
                 if(n == 1)
                 {
                    fputc('1', file);
                 }
                 else
                 {
                    fputc('0', file);        
                 }
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
