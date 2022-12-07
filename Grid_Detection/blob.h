#ifndef _BLOB_H_
#define _BLOB_H_

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <stdio.h>


typedef struct Blob
{   int xmax;
    int ymax;
    int xmin;
    int ymin;
    int* list;
} Blob; 

typedef struct Blob_list
{
    Blob* list;
    size_t size;
    int nb_blob;
} Blob_list;

struct Blob* init_blob(int x,int y);
void free_Blob_list(struct Blob_list* b);
void __remove(struct Blob* l,int i, int len);
void blob_pop(struct Blob_list *b, int i);
int max(int a,int b);
int min(int a,int b);
int count(Uint32 *tab, int w, int h, int x, int y, int n);
int max_density(Uint32 *t, int w, int h, int zone);
void update_blob(struct Blob *blob,int x, int y);
void update_blob_between_blob(struct Blob *blob1,struct Blob *blob2);
char is_in_blob_reach(struct Blob* blob, int t, int x, int y);
struct Blob* add_blob(Blob_list* blob_list , int x, int y);
int blob_size(struct Blob *blob);
int inter(int max1, int min1, int max2, int min2, int t);
struct Blob* merge_blobs(struct Blob_list* blobs);
struct Blob_list* generateBlob(SDL_Surface* s);
void show_blobs(SDL_Surface* s, Blob_list* bloblist);
SDL_Surface* crop(SDL_Surface* s, Blob* blob);
void apply_blob_crop(SDL_Surface* s);

#endif
