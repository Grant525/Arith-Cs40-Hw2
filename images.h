#ifndef IMAGES_H
#define IMAGES_H

#include <stdio.h>
#include <pnm.h>

Pnm_ppm read_ppm(FILE *input);

Pnm_ppm trim(Pnm_ppm pixmap);

void read_header(FILE *input, unsigned *width, unsigned *height);

A2Methods_UArray2 read_codewords(FILE *input, unsigned width, unsigned height);

#endif

