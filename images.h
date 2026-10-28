#ifndef IMAGES_H
#define IMAGES_H

#define DENOMINATOR 255

#include <stdio.h>
#include <stdint.h>
#include <pnm.h>
#include <a2methods.h>

Pnm_ppm read_ppm(FILE *input);

Pnm_ppm trim(Pnm_ppm pixmap);

void read_header(FILE *input, unsigned *width, unsigned *height);

A2Methods_UArray2 read_codewords(FILE *input, unsigned width, unsigned height);

Pnm_ppm create_pixmap(unsigned width, unsigned height);

void write_codeword(uint64_t codeword);

void write_header(unsigned width, unsigned height);

void write_stdout(Pnm_ppm pixmap);

#endif

