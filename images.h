/**************************************************************
 *
 *                     images.h
 *
 *        Assignment: CS 40 HW 4, Arith
 *        Authors:    Micheal Gonzales (mgonza20),
 *                    Grant Caile (gcaile01)
 *        Date:       October 28, 2026
 *
 *        Interface for image input and output: reading and trimming
 *        PPM images, reading and writing the compressed-image header
 *        and codewords, and writing a decompressed PPM to stdout.
 *
 **************************************************************/
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
