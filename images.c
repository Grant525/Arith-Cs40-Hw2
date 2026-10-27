#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
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
    assert(new_width > 0);
    assert(new_height > 0);
    const struct A2Methods_T *methods = pixmap->methods;
    int size = pixmap->methods->size(pixmap->pixels);
    A2Methods_UArray2 array = methods->new(new_width, new_height, size);
    for (int row = 0; row < new_height; row++) {
        for (int column = 0; column < new_width; column++) {
            A2Methods_Object *source = methods->at(pixmap->pixels, column, row);
            A2Methods_Object *destination = methods->at(array, column, row);
            memcpy(destination, source, size);
        }
    }
    A2Methods_UArray2 old = pixmap->pixels;
    methods->free(&old);
    pixmap->pixels = array;
    pixmap->width = new_width;
    pixmap->height = new_height;
    return pixmap;
}

void read_header(FILE *input, unsigned *width, unsigned *height) {
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", 
        width, height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');
}

A2Methods_UArray2 read_codewords(FILE *input, unsigned width, unsigned height) {
    A2Methods_T methods = uarray2_methods_plain;
    A2Methods_UArray2 words = methods->new(width / 2, height / 2, sizeof(uint64_t));
    for (unsigned row = 0; row < height / 2; row++) {
        for (unsigned column = 0; column < width / 2; column++) {
            uint64_t word = 0;
            for (int k = 0; k < 4; k++) {
                int c = getc(input);
                assert(c != EOF);
                word = word << 8;
                uint64_t new_c = c;
                word = word | new_c;
            }
            *(uint64_t *)methods->at(words, column, row) = word;
        }
    }
    return words;
}


