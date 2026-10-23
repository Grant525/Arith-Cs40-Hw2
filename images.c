#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assert.h"
#include "images.h"
#include "a2plain.h"

Pnm_ppm read_ppm(FILE *input) {
    A2Methods_T methods = uarray2_methods_plain; 
    Pnm_ppm pixmap = Pnm_ppmread(input, methods);
    assert(pixmap != NULL);
    return pixmap;
}

Pnm_ppm trim(Pnm_ppm pixmap) {
    int width = pixmap->width;
    int height = pixmap->height;
    int new_height;
    int new_width;
    if (height % 2 != 0) {
        new_height = height - 1;
    }
    else {
        new_height = height;
    }
    if (width % 2 != 0) {
        new_width = width - 1;
    }
    else {
        new_width = width;
    }
    if (new_width == width && new_height == height) {
        return pixmap;
    }
    A2Methods_T methods = pixmap->methods;
    int size = pixmap->methods->size(pixmap->pixels);
    A2Methods_UArray2 array = methods->new(new_width, new_height, size);
    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            A2Methods_Object source = methods->at(pixmap->pixels, j, r);
            A2Methods_Object destination = methods->at(array, j, r);
            memcpy(destination, source, size);
        }
    }
    //
    return pixmap;
}
