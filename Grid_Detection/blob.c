#include "blob.h"
Blob* init_blob(int x,int y)
{
    Blob* blob = malloc(sizeof(Blob));
    blob->xmax = x;
    blob->ymax = y;
    blob->xmin = x;
    blob->ymin = y;
    return blob;
}

void free_Blob_list(Blob_list* b)
{
    free(b->list);
    free(b);
}

void __remove(Blob* l,int i, int len)
{
    for (; i < len - 1; ++i)
    {
        *(l+i) = *(l+i+1);
    }
}

void blob_pop(Blob_list *b, int i)
{
    __remove(b->list, i, b->size);
    b->nb_blob--;
}


int max(int a,int b)
{
    if(a > b)
        return a;
    return b;
}

int min(int a,int b)
{
    if(a < b)
        return a;
    return b;
}

int count(Uint32 *tab, int w, int h, int x, int y, int n) 
{
  int res = 0;
  int start_line = y - n / 2;
  int start_col = x - n / 2;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (start_col + j < w && start_col + j >= 0 && start_line + i < h &&
          start_line + i >= 0)
        res += *(tab + (start_line + i) * w + (start_col + j));
    }
  }
  return res;
}

int max_density(Uint32 *t, int w, int h, int zone)
{
    int max = 0;
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            if (t[x + w * y] != 0)
            {
                int tmp = count(t, w, h, x, y, zone);
                max = max < tmp ? tmp : max;
            }
        }
    }
    return max;
}

void update_blob(Blob *blob,int x, int y)
{
    blob->xmax = max(blob->xmax, x);
    blob->ymax = max(blob->ymax, y);
    blob->xmin = min(blob->xmin, x);
    blob->ymin = min(blob->ymin, y);
}

void update_blob_between_blob(Blob *blob1,struct Blob *blob2)
{
    blob1->xmax = max(blob1->xmax, blob2->xmax);
    blob1->ymax = max(blob1->ymax, blob2->ymax);
    blob1->xmin = min(blob1->xmin, blob2->xmin);
    blob1->ymin = min(blob1->ymin, blob2->ymin);
}



char is_in_blob_reach(Blob* blob, int t, int x, int y)
{
    return (x >= blob->xmin - t
         && x <= blob->xmax + t
         && y >= blob->ymin - t
         && y <= blob->ymax + t);
}

Blob* add_blob(Blob_list* blob_list , int x, int y)
{
    if (blob_list->size == blob_list->nb_blob)
    {
        blob_list->size *= 2;
        blob_list->list = realloc(blob_list->list,blob_list->size*sizeof(Blob));
    }
    Blob* b = init_blob(x,y);
    *(blob_list->list + blob_list->nb_blob) = *b;
    blob_list->nb_blob++;
    return b;
}

int blob_size(Blob *blob)
{
    return ((blob->xmax - blob->xmin) * (blob->ymax - blob->ymin));
}


int inter(int max1, int min1, int max2, int min2, int t)
{
    if (max2 < min1)
        return min1-max2 <= t ? 1 : 0;
    else if (min2 > max1)
        return min2-max1 <= t ? 1 : 0;
    return 1;
}


Blob* merge_blobs(struct Blob_list* blobs)
{
    int acc = 1;
    while (acc != 0)
    {
        acc = 0;
        for (int i = 0; i < blobs->nb_blob && acc == 0; ++i)
        {
            for (int j = blobs->nb_blob; j >= 0; --j) //si j < i faut pas continuer apres une intersect
            {
                if (j == i)
                    continue;
                int xmax1 = blobs->list[i].xmax;
                int xmin1 = blobs->list[i].xmin;
                int xmax2 = blobs->list[j].xmax;
                int xmin2 = blobs->list[j].xmin;

                int ymax1 = blobs->list[i].ymax;
                int ymin1 = blobs->list[i].ymin;
                int ymax2 = blobs->list[j].ymax;
                int ymin2 = blobs->list[j].ymin;

                if (inter(xmax1, xmin1, xmax2, xmin2, 2)!= 0)
                {
                    if(inter(ymax1, ymin1, ymax2, ymin2, 2) != 0)
                    {
                        update_blob_between_blob(&blobs->list[i], &blobs->list[j]);
                        blob_pop(blobs, j);
                        acc++;

                        if (j < i)
                            break;
                    }

                }
            }
        }
    }

    int max = blob_size(blobs->list);
    Blob* blob_max = blobs->list;

    for (int i = 1; i < blobs->nb_blob; ++i)
    {
        int tmp = blob_size(blobs->list+i);
        if (tmp > max)
        {
            max = tmp;
            blob_max = blobs->list+i;
        }
    }
    Blob* res = malloc(sizeof(Blob_list));
    *res = *blob_max;
    free_Blob_list(blobs);
    return res;
}

struct Blob_list* generateBlob(SDL_Surface* s)
{
    int w = s->w;
    int h = s->h;
    Uint32* pixels = s->pixels;

    Blob_list* bloblist = malloc(sizeof(Blob_list));
    bloblist->list = malloc(sizeof(Blob) * 8);
    bloblist->nb_blob = 0;
    bloblist->size = 8;

    int zone = max(w,h)*15/100;
    int max = max_density(pixels,w,h,zone);
    int threshold = (max * 98) / 100;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (pixels[x + w * y] != 0 &&
                    count(pixels, w, h, x, y, zone) > threshold)
            {
                char found = 0;
                for (int i = 0; i < bloblist->nb_blob; i++)
                {
                    if (is_in_blob_reach(&bloblist->list[i],zone/10,x,y))
                    {
                        update_blob(&bloblist->list[i], x, y);
                        found = 1;
                        break;
                    }
                }

                if (!found)
                {
                    add_blob(bloblist, x, y);
                }
            }
        }
    }
    return bloblist;
}

void show_blobs(SDL_Surface* s, Blob_list* bloblist)
{
    SDL_LockSurface(s);
    for (int i = 0; i < bloblist->nb_blob; ++i)
    {
        Blob b = bloblist->list[i];
        SDL_Rect* rect = malloc(sizeof(SDL_Rect));
        rect->x = b.xmin;
        rect->y = b.ymin;
        rect->w = b.xmax - b.xmin;
        rect->h = b.ymax - b.ymin;
        Uint32 color = SDL_MapRGB(s->format,255,0,0);
        SDL_FillRect(s,rect,color);
    }
    SDL_UnlockSurface(s);
    SDL_SaveBMP(s,"test_show_blob.bmp");
}

SDL_Surface* crop(SDL_Surface* s, Blob* blob)
{
    int w = blob->xmax - blob->xmin;
    int h = blob->ymax - blob->ymin;
    SDL_Surface *res_surface = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);
    Uint32* pixels = s->pixels;
    Uint32* new_pixels = res_surface->pixels;

    SDL_LockSurface(s);
    SDL_LockSurface(res_surface);


    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            int index = blob->xmin + x + (y + blob->ymin) * s->w;
            new_pixels[x + w * y] = pixels[index];
        }
    }

    SDL_UnlockSurface(s);
    SDL_UnlockSurface(res_surface);
    return res_surface;

}

void apply_blob_crop(SDL_Surface* s)
{
    Blob_list* bloblist = generateBlob(s);
    //show_blobs(s,bloblist,"result_image/blobs.bmp");
    Blob* final_blob = merge_blobs(bloblist);
    SDL_Surface* cropped = crop(s,final_blob);
    SDL_FreeSurface(s);
    SDL_SaveBMP(cropped,"test_blob.bmp");
    SDL_FreeSurface(cropped);
}

