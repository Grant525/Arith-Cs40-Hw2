/**************************************************************
 *
 *                     conversions.c
 *
 *        Assignment: CS 40 HW 4, Arith
 *        Authors:    Micheal Gonzales (mgonza20),
 *                    Grant Caile (gcaile01)
 *        Date:       October 28, 2026
 *
 *        Implementation of conversions.h. Converts scaled-integer
 *        RGB pixels to floating-point component video (Y, Pb, Pr)
 *        when compressing, and back to scaled RGB when
 *        decompressing.
 *
 **************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "conversions.h"
#include "compress40.h"
#include "uarray2.h"

/********** create_float_pixmap ********
 *
 * Creates an array to hold an image in component video form, using
 * 2x2 blocks so each block's pixels are mapped together
 *
 * Parameters:
 *        unsigned width:      number of columns
 *        unsigned height:     number of rows
 *        A2Methods_T methods: methods suite to create the array with
 *
 * Return: a new width x height array of struct pixmap_float; the caller
 *         must free it with methods->free
 *
 * Expects
 *        width and height are at least 1
 * Notes:
 *        Will give Runtime Error if width or height is 0 or allocation fails
 ************************/
A2Methods_UArray2 create_float_pixmap(unsigned width, unsigned height,
                                      A2Methods_T methods)
{
        A2Methods_UArray2 pixmap_new = methods->new_with_blocksize(
                width, height, sizeof(struct pixmap_float), 2);
        return pixmap_new;
}

/* what you had
UarrayT pixmap_new = Uarray2_new(width, height, sizeof(pixmap->pixel))
for (int row_num, row_num < height, row_num++){
        for (int col_num, col_num < width, col_num++){
                struct pixmap_float pf;
                *(struct pixmap_float *)methods->at(pixmap_new, col_num, row_num) = pf;
        }
}
*/

/* Convert_to_components() added as well */

/********** Convert_to_float ********
 *
 * Converts every pixel of an RGB image to component video
 *
 * Parameters:
 *        A2Methods_UArray2 pixmap_new: destination array of
 *                                      struct pixmap_float, same size
 *                                      as pixmap
 *        Pnm_ppm pixmap:               the source image
 *        A2Methods_T methods:          methods for pixmap_new
 *
 * Return: none
 *
 * Expects
 *        pixmap_new and pixmap are not NULL and have the same size
 * Notes:
 *        Overwrites every element of pixmap_new
 ************************/
void Convert_to_float(A2Methods_UArray2 pixmap_new, Pnm_ppm pixmap,
                      A2Methods_T methods)
{
        int width = pixmap->width;
        int height = pixmap->height;
        const struct A2Methods_T *old_methods = pixmap->methods;

        for (int row_num = 0; row_num < height; row_num++) {
                for (int col_num = 0; col_num < width; col_num++) {
                        struct Pnm_rgb *rgb = old_methods->at(pixmap->pixels,
                                                              col_num,
                                                              row_num);
                        double r = (double)rgb->red / pixmap->denominator;
                        double b = (double)rgb->blue / pixmap->denominator;
                        double g = (double)rgb->green / pixmap->denominator;

                        double y = 0.299 * r + 0.587 * g + 0.114 * b;
                        double pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
                        double pr = 0.5 * r - 0.418688 * g - 0.081312 * b;

                        struct pixmap_float *pf = methods->at(pixmap_new,
                                                              col_num,
                                                              row_num);
                        pf->y = y;
                        pf->pb = pb;
                        pf->pr = pr;
                }
        }
}

/********** Convert_to_RGB ********
 *
 * Converts one component video pixel to scaled-integer RGB
 *
 * Parameters:
 *        double y:        luma
 *        double pb:       blue-difference chroma
 *        double pr:       red-difference chroma
 *        int denominator: maximum value of an RGB channel
 *
 * Return: the RGB pixel, each channel in 0..denominator
 *
 * Expects
 *        denominator is positive
 ************************/
struct Pnm_rgb Convert_to_RGB(double y, double pb, double pr, int denominator)
{
        double r = 1.0 * y + 0.0 * pb + 1.402 * pr;
        double g = 1.0 * y - 0.344136 * pb - 0.714136 * pr;
        double b = 1.0 * y + 1.772 * pb + 0.0 * pr;

        struct Pnm_rgb p;
        quanitze_RGB(&p, r, g, b, denominator);
        return p;
}

/********** quanitze_RGB ********
 *
 * Scales red, green, and blue values in [0, 1] to integers in
 * 0..denominator and stores them in an RGB pixel
 *
 * Parameters:
 *        struct Pnm_rgb *p: the pixel to fill
 *        double r, g, b:    channel values; out-of-range values are
 *                           clamped to [0, 1]
 *        int denominator:   maximum value of an RGB channel
 *
 * Return: none
 *
 * Expects
 *        p is not NULL and denominator is positive
 * Notes:
 *        Overwrites *p
 ************************/
void quanitze_RGB(struct Pnm_rgb *p, double r, double g, double b,
                  int denominator)
{
        if (r < 0) {
                r = 0;
        }
        if (b < 0) {
                b = 0;
        }
        if (g < 0) {
                g = 0;
        }
        if (r > 1) {
                r = 1;
        }
        if (b > 1) {
                b = 1;
        }
        if (g > 1) {
                g = 1;
        }
        p->red = (unsigned) round(r * denominator);
        p->blue = (unsigned) round(b * denominator);
        p->green = (unsigned) round(g * denominator);
}
