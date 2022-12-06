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

/*effectue une rotation centrale d'angle en degré, alloue automatiquement la mémoire*/
SDL_Surface* SDL_RotationCentral(SDL_Surface* origine, float angle)
{
    SDL_Surface* destination;
    int i;
    int j;
    Uint32 couleur;
    int mx, my;
    int bx, by;
    float angle_radian;

    /* détermine la valeur en radian de l'angle*/
    angle_radian = angle * M_PI / 180.0;

    /*
        * alloue la mémoire à l'espace de destination, attention,
        * la surface est de même taille
        */
    destination = SDL_CreateRGBSurface(SDL_SWSURFACE, origine->w, origine->h, origine->format->BitsPerPixel,
            origine->format->Rmask, origine->format->Gmask, origine->format->Bmask, origine->format->Amask);

    /*on vérifie que la mémoire a été allouée*/
    if(destination==NULL)
        return NULL;

    /* pour simplifier les notations*/
    mx = origine->w/2;
    my = origine->h/2;

    for(j=0;j<origine->h;j++)
        for(i=0;i<origine->w;i++)
        {
            /* on détermine la valeur de pixel qui correspond le mieux pour la position
                * i,j de la surface de destination */

            /* on détermine la meilleure position sur la surface d'origine en appliquant
                * une matrice de rotation inverse
                */
            bx = (int) (cos(angle_radian) * (i-mx) + sin(angle_radian) * (j-my)) + mx;
            by = (int) (-sin(angle_radian) * (i-mx) + cos(angle_radian) * (j-my)) + my;
            /* on vérifie que l'on ne sort pas des bords*/
            if (bx>=0 && bx< origine->w && by>=0 && by< origine->h)
            {
                couleur = SDL_LirePixel(origine, bx, by);
                SDL_EcrirePixel(destination, i, j, couleur);
            }
        }

    return destination;
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

