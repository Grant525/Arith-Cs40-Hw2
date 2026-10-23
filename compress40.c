#include <stdio.h>
#include "compress40.h"
#include "images.h"

void compress40(FILE *input) {
    Pnm_ppm pixmap = read_ppm(input);
    Pnm_ppmfree(&pixmap);
}

void decompress40(FILE *input) {
    (void)input;
}