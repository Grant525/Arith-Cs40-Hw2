/**************************************************************
 *
 *                     compress40.c
 *
 *        Assignment: CS 40 HW 4, Arith
 *        Authors:    Micheal Gonzales (mgonza20),
 *                    Grant Caile (gcaile01)
 *        Date:       October 28, 2026
 *
 *        Implementation of compress40.h. Ties together the steps of
 *        compression (read, trim, convert to component video, pack
 *        each 2x2 block into a codeword) and of decompression (read
 *        codewords, unpack them into pixels, write a PPM).
 *
 **************************************************************/

#include <stdio.h>
#include "compress40.h"
#include "images.h"
#include "conversions.h"
#include "codeword.h"
#include "block.h"
#include "a2plain.h"
#include "a2blocked.h"
#include <stdint.h>

/********** compress40 ********
 *
 * Reads a PPM image and writes its compressed form to stdout
 *
 * Parameters:
 *        FILE *input: open stream holding a PPM image
 *
 * Return: none
 *
 * Expects
 *        input is not NULL and holds a valid PPM at least 2x2 in size
 * Notes:
 *        An odd width or height is trimmed by one before compressing
 *        Will give Runtime Error if the image can't be read or is too small
 ************************/
void compress40(FILE *input)
{
        A2Methods_T methods = uarray2_methods_blocked;
        Pnm_ppm pixmap = read_ppm(input);
        pixmap = trim(pixmap);
        A2Methods_UArray2 pixmap_new = create_float_pixmap(pixmap->width,
                                                           pixmap->height,
                                                           methods);
        Convert_to_float(pixmap_new, pixmap, methods);
        struct pixmap_block p;
        p.count = 0;
        write_header(pixmap->width, pixmap->height);
        methods->map_default(pixmap_new, chroma_conversion, &p);
        methods->free(&pixmap_new);
        Pnm_ppmfree(&pixmap);
}

/********** decompress40 ********
 *
 * Reads a compressed image and writes it to stdout as a PPM
 *
 * Parameters:
 *        FILE *input: open stream holding a compressed image
 *
 * Return: none
 *
 * Expects
 *        input is not NULL and holds a header followed by one codeword
 *        per 2x2 block
 * Notes:
 *        Will give Runtime Error if the header is malformed or the input ends early
 ************************/
void decompress40(FILE *input)
{
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
