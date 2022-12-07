#include "rotation.h"

inline Uint32 SDL_LirePixel(SDL_Surface* surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch(bpp)
    {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *)p;
        default:
            return 0;
    }
}


/*permet d'écrire un pixel au position x,y*/
inline void SDL_EcrirePixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch(bpp) {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:

            *(Uint32 *)p = pixel;
            break;
    }

}

SDL_Surface *rotation(SDL_Surface *image, double angle){
    //Get the image dimensions
    int w = image->w;
    int h = image->h;

    //Create a new surface to hold the rotated image
    SDL_Surface *rotated_image; 
    rotated_image = SDL_CreateRGBSurfaceWithFormat(0, image->w, image->h, image->format->BitsPerPixel, image->format->format);

    double angle_rad = angle * (M_PI / 180); //convert the angle from degrees to radians
    int center_x = w/2;
    int center_y = h/2;
    double cos_angle = cos(angle_rad);
    double sin_angle = sin(angle_rad);
    for(int x = 0; x < w;x++){
        for(int y = 0; y < h; y++){
            // Computes the offsets from the center of the image
            double xOff = (x - center_x);
            double yOff = (y - center_y);
            // Compute the coordinates from the image
            int new_x = round(xOff * cos_angle + yOff * sin_angle + center_x);
            int new_y = round(yOff * cos_angle - xOff * sin_angle + center_y);
            // Check if the new coordinates are within the image
            if(0 <= new_x && new_x < w && 0 <= new_y && new_y < h){
                // Copy the pixel from the old image to the new image
                Uint32 pixel = SDL_LirePixel(image, x, y);
                SDL_EcrirePixel(rotated_image, new_x, new_y, pixel);
            }
        }
    }
    SDL_FreeSurface(image);
    return rotated_image;
}


//Rotation_shearing, rotate an image and shear it
//@param image: the image to rotate
//@param angle: the angle to rotate the image by
//@return: the rotated and sheared image

SDL_Surface *Rotation_shearing(SDL_Surface *image, double angle){
    //Get the image dimensions
    int w = image->w;
    int h = image->h;
    //Create a new surface to hold the rotated image
    SDL_Surface *rotated_image; 
    rotated_image = SDL_CreateRGBSurface(0, w, h, 32,0,0,0,0);

    double angle_rad = angle * (M_PI / 180); //convert the angle from degrees to radians
    int center_x = w/2;
    int center_y = h/2;
    double sin_angle = sin(angle_rad);
    double tan_angle = tan(angle_rad/2);
    for(int x = 0; x < w;x++){
        for(int y = 0; y < h; y++){
            // Computes the offsets from the center of the image
            double xOff = (x - center_x);
            double yOff = (y - center_y);
            // Compute the coordinates from the image
            //shear 1
            int new_x = round(xOff - yOff* tan_angle);
            int new_y = yOff;
            //shear 2
            new_y = round(new_x * sin_angle + new_y);
            //shear 3
            new_x = round(new_x - new_y*tan_angle);

            new_y += center_y;
            new_x += center_x;
            // Check if the new coordinates are within the image
            if(0 <= new_x && new_x < w && 0 <= new_y && new_y < h){
                // Copy the pixel from the old image to the new image
                Uint32 pixel = SDL_LirePixel(image, x, y);
                SDL_EcrirePixel(rotated_image, new_x, new_y, pixel);
            }
        }
    }
    return rotated_image;
}
float get_angle(char* s)
{
    float angle = 0;
    size_t i = 0;
    int is_neg = 0;
    if(s[0] != '-' && (s[0] < '0' || s[0] > '9'))
        errx(EXIT_FAILURE, "Put an valid angle ");

    if(s[0] == '-')
    {
        is_neg = 1;
        i++;
    }

    for(; s[i] != 0; i++)
    {
        if ('0' <= s[i] && s[i] <= '9')
            angle = angle *10. + (s[i] - '0');
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
    return angle;
}

