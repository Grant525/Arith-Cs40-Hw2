/**************************************************************
 *
 *                     images.c
 *
 *        Assignment: CS 40 HW 4, Arith
 *        Authors:    Micheal Gonzales (mgonza20),
 *                    Grant Caile (gcaile01)
 *        Date:       October 28, 2026
 *
 *        Implementation of images.h. Handles all reading and
 *        writing of images: PPM input and output through pnm.h,
 *        and the compressed format, which is the header
 *        "COMP40 Compressed image format 2\n<width> <height>\n"
 *        followed by one big-endian 32-bit codeword per 2x2 block in
 *        row-major order.
 *
 **************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "assert.h"
#include "images.h"
#include "a2plain.h"
#include "bitpack.h"

/********** read_ppm ********
 *
 * Reads a PPM image into a plain (unblocked) pixmap
 *
 * Parameters:
 *        FILE *input: open stream holding a PPM image
 *
 * Return: the image; the caller must free it with Pnm_ppmfree
 *
 * Expects
 *        input is not NULL and holds a valid PPM
 * Notes:
 *        Will give Runtime Error if the image can't be read
 ************************/
Pnm_ppm read_ppm(FILE *input)
{
        A2Methods_T methods = uarray2_methods_plain;
        Pnm_ppm pixmap = Pnm_ppmread(input, methods);
        assert(pixmap != NULL);
        return pixmap;
}

/********** trim ********
 *
 * Makes an image's width and height even by dropping the last column
 * and/or row if needed
 *
 * Parameters:
 *        Pnm_ppm pixmap: the image to trim
 *
 * Return: pixmap, with its pixels replaced if it was trimmed
 *
 * Expects
 *        pixmap is not NULL and at least 2x2 in size
 * Notes:
 *        Frees pixmap's old pixel array if it is trimmed
 *        Will give Runtime Error if trimming would leave a width or height of 0
 ************************/
Pnm_ppm trim(Pnm_ppm pixmap)
{
        int width = pixmap->width;
        int height = pixmap->height;
        int new_height;
        int new_width;
        if (height % 2 != 0) {
                new_height = height - 1;
        } else {
                new_height = height;
        }
        if (width % 2 != 0) {
                new_width = width - 1;
        } else {
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
                        A2Methods_Object *source =
                                methods->at(pixmap->pixels, column, row);
                        A2Methods_Object *destination =
                                methods->at(array, column, row);
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

/********** read_header ********
 *
 * Reads the header of a compressed image
 *
 * Parameters:
 *        FILE *input:      open stream holding a compressed image
 *        unsigned *width:  where to store the image's width
 *        unsigned *height: where to store the image's height
 *
 * Return: none
 *
 * Expects
 *        input, width, and height are not NULL
 * Notes:
 *        Sets *width and *height; leaves input at the first codeword
 *        Will give Runtime Error if the header is malformed
 ************************/
void read_header(FILE *input, unsigned *width, unsigned *height)
{
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u",
                          width, height);
        assert(read == 2);
        int c = getc(input);
        assert(c == '\n');
}

/********** read_codewords ********
 *
 * Reads every codeword of a compressed image
 *
 * Parameters:
 *        FILE *input:     open stream positioned at the first codeword
 *        unsigned width:  width of the image in pixels
 *        unsigned height: height of the image in pixels
 *
 * Return: a (width / 2) x (height / 2) plain array of uint64_t
 *         codewords; the caller must free it
 *
 * Expects
 *        input is not NULL and holds (width / 2) * (height / 2)
 *        codewords
 * Notes:
 *        Will give Runtime Error if the input ends early
 ************************/
A2Methods_UArray2 read_codewords(FILE *input, unsigned width, unsigned height)
{
        A2Methods_T methods = uarray2_methods_plain;
        A2Methods_UArray2 words = methods->new(width / 2, height / 2,
                                               sizeof(uint64_t));
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

/********** create_pixmap ********
 *
 * Creates an empty PPM image with denominator DENOMINATOR
 *
 * Parameters:
 *        unsigned width:  width of the image
 *        unsigned height: height of the image
 *
 * Return: the new image; the caller must free it with Pnm_ppmfree
 *
 * Expects
 *        width and height are at least 1
 * Notes:
 *        Will give Runtime Error if memory allocation fails
 ************************/
Pnm_ppm create_pixmap(unsigned width, unsigned height)
{
        A2Methods_T methods = uarray2_methods_plain;

        A2Methods_UArray2 pixels = methods->new(width, height,
                                                sizeof(struct Pnm_rgb));
        assert(pixels != NULL);
        Pnm_ppm pixmap = malloc(sizeof(*pixmap));
        assert(pixmap != NULL);

        pixmap->width = width;
        pixmap->height = height;
        pixmap->denominator = DENOMINATOR;
        pixmap->pixels = pixels;
        pixmap->methods = methods;

        return pixmap;
}

/********** write_codeword ********
 *
 * Writes a 32-bit codeword to stdout in big-endian order
 *
 * Parameters:
 *        uint64_t codeword: the codeword, in its low 32 bits
 *
 * Return: none
 ************************/
void write_codeword(uint64_t codeword)
{
        for (int lsb = 24; lsb >= 0; lsb -= 8) {
                putchar(Bitpack_getu(codeword, 8, lsb));
        }
}

/********** write_header ********
 *
 * Writes the header of a compressed image to stdout
 *
 * Parameters:
 *        unsigned width:  width of the image
 *        unsigned height: height of the image
 *
 * Return: none
 ************************/
void write_header(unsigned width, unsigned height)
{
        printf("COMP40 Compressed image format 2\n%u %u", width, height);
        putchar('\n');
}

/********** write_stdout ********
 *
 * Writes a PPM image to stdout
 *
 * Parameters:
 *        Pnm_ppm pixmap: the image
 *
 * Return: none
 *
 * Expects
 *        pixmap is not NULL
 * Notes:
 *        Will give Runtime Error if pixmap is NULL
 ************************/
void write_stdout(Pnm_ppm pixmap)
{
        assert(pixmap != NULL);
        Pnm_ppmwrite(stdout, pixmap);
}
