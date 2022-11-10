#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>

void draw_digits(SDL_Surface surface, char* sudoku_file, char* result_file)
{
    FILE *sudoku = fopen(sudoku_file, "r");
    if(sudoku == NULL)
    {
        printf("Erreur fopen\n");
        return 1;
    }

    FILE *result = fopen(result_file, "r");
    if(result  == NULL)
    {
        printf("Erreur fopen\n");
        return 1;
    }
    SDL_LockSurface(surface);





    SDL_UnlockSurface(surface);
    fclose(sudoku);
    fclose(result);
}



int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 4)
        errx(EXIT_FAILURE, "Usage: image-file");

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

    draw_digits(surface, argv[2], argv[3]);

    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
