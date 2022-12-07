#include "blob.h"


int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");
    SDL_Surface* s = load_image(argv[1]);

    //Call the hough function
    int* tab = hough_function(s);
    SDL_SaveBMP(s,"merde.bmp");
    apply_bob_crop(argv[1],"merde.bmp");

    // Destroys the objects.
    SDL_FreeSurface(s);
    SDL_Quit();


    return EXIT_SUCCESS;
}
