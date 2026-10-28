#include <stdio.h>
#include "compress40.h"
#include "images.h"
#include "conversions.h"
#include "codeword.h"
#include "block.h"
#include "a2plain.h"
#include "a2blocked.h"
#include <stdint.h>

void compress40(FILE *input) {
    A2Methods_T methods = uarray2_methods_blocked;
    Pnm_ppm pixmap = read_ppm(input);
    pixmap = trim(pixmap);
    A2Methods_UArray2 pixmap_new = create_float_pixmap(pixmap->width, pixmap->height, methods);
    Convert_to_float(pixmap_new, pixmap, methods);
    struct pixmap_block p;
    p.count = 0;
    write_header(pixmap->width, pixmap->height);
    methods->map_default(pixmap_new , chroma_conversion, &p);
    methods->free(&pixmap_new);
    Pnm_ppmfree(&pixmap);
}

void decompress40(FILE *input) {
    unsigned width;
    unsigned height;
    A2Methods_T methods = uarray2_methods_plain;
    read_header(input, &width, &height);
    A2Methods_UArray2 words = read_codewords(input, width, height);
    Pnm_ppm pixmap = create_pixmap(width, height);
    unpack_codeword(pixmap, words, methods);
    methods->free(&words);
    write_stdout(pixmap);
    Pnm_ppmfree(&pixmap);
}