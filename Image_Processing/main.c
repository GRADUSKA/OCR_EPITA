#include "main.h"
#define KX (int[]){-1, 0, 1,-2, 0, 2,-1, 0, 1}

#define KY (int[]){1, 2, 1,0, 0, 0,-1, -2, -1}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 3 && argc != 4)
        errx(EXIT_FAILURE, "Usage: image-file , image proccessing fonction, optional argument(for rotation)");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* s = load_image(argv[1]);
    if(s == NULL)
        errx(EXIT_FAILURE,"%s",SDL_GetError());
    char* a = argv[2];
    if (strcmp(a, "--grayscale") == 0) 
    {
        surface_to_grayscale(s);
        SDL_SaveBMP(s,"Grayscale.bmp");
    } 
    else if (strcmp(a, "--gaussian") == 0) 
    {
        ApplyGaussian(s);
        SDL_SaveBMP(s,"Gaussian.bmp");
    } 
    else if (strcmp(a, "--canny") == 0) 
    {
        int* Gx = Convolution(s,KX);
        int* Gy = Convolution(s,KY);
        double* theta = direction_grad(Gx, Gy,s->w*s->h);
        free(Gx);
        free(Gy);
        ApplySobel(s);
        double* nms = non_max_suppr(s,theta);
        Uint32* pixels = s->pixels;
        SDL_LockSurface(s);
        for(int i = 0 ;i < s->w*s->h;i++)
        {
            pixels[i]= SDL_MapRGB(s->format, nms[i],nms[i],nms[i]);
        }
        SDL_UnlockSurface(s);
        hyperthesis(s);
        SDL_SaveBMP(s, "Canny.bmp");
    }
    else if (strcmp(a,"--all") == 0)
    {
        surface_to_grayscale(s);
        ApplyGaussian(s);
        int* Gx = Convolution(s,KX);
        int* Gy = Convolution(s,KY);
        double* theta = direction_grad(Gx, Gy,s->w*s->h);
        free(Gx);
        free(Gy);
        ApplySobel(s);
        double* nms = non_max_suppr(s,theta);
        Uint32* pixels = s->pixels;
        SDL_LockSurface(s);
        for(int i = 0 ;i < s->w*s->h;i++)
        {
            pixels[i]= SDL_MapRGB(s->format, nms[i],nms[i],nms[i]);
        }
        SDL_UnlockSurface(s);
        hyperthesis(s);
        SDL_SaveBMP(s, "ImageProssessing.bmp");

    }
    else if (strcmp(a, "--rotation") == 0)
    {
        if(argc == 3)
            errx(EXIT_FAILURE,"Put an angle");

        float a = get_angle(argv[3]);
        SDL_Surface* res = SDL_RotationCentral(s,a);    
        SDL_SaveBMP(res,"Rotation.bmp");
    }


    SDL_FreeSurface(s);
    SDL_Quit();
    return EXIT_SUCCESS;
}
