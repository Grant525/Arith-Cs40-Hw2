#include <stdio.h>
#include "compress40.h"
#include "images.h"
#include "conversion.h"
#include "codeword.h"
#include "block.h"
#include "a2plain.h"

void compress40(FILE *input) {
    A2Methods_T methods = uarray2_methods_blocked;
    Pnm_ppm pixmap = read_ppm(input);
    A2Methods_T pixmap = trim(input);
    A2Methods_T pixmap_new = create_float_pixmap(width, height, etc)
    Convert_to_float(*pixmap_new, pixmap)
    methods->map(chroma_conversion(pixmap_new, (int col, int row, A2Methods_T pixmap, void *elem, void *cl), void *cl))
    Pnm_ppmfree(&pixmap);
    //free new pixmap?
}

void decompress40(FILE *input) {
    unsigned width;
    unsigned height;
    A2Methods_T methods = uarray2_methods_plain;
    read_header(input, &width, &height);
    A2Methods_UArray2 words = read_codewords(input, width, height);
    Pnm_ppm pixmap = create_pixmap(width, height);
    for (unsigned row = 0; row < height / 2; row ++) {
        for (unsigned col = 0; col < width / 2; col++) {
            uint64_t word = *(uint64_t *)methods->at(words, col, row);
            unpack_codewords();
            //dequantize
            //inverse dct
            //store in pixmap
        }
    }
    methods->free(&words);
    write_stdout(pixmap);
    Pnm_ppmfree(&pixmap);
}