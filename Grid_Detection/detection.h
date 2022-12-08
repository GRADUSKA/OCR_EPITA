#include "../Image_Processing/use.h"
#include "../Image_Processing/image_process.h"
#include "../Image_Processing/rotation.h"
#include "blob.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
typedef struct vector
{
    size_t capacity;
    size_t size;
    int *data;
}vector;

