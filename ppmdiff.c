/**************************************************************
 *
 *                     ppmdiff.c
 *
 *        Assignment: CS 40 HW 4, Arith
 *        Authors:    Micheal Gonzales (mgonza20),
 *                    Grant Caile (gcaile01)
 *        Date:       October 28, 2026
 *
 *        Testing tool that compares two PPM images and prints the
 *        root mean square difference of their pixels (0.0 means the
 *        images are identical). Pixels outside the smaller of the
 *        two images are ignored.
 *
 **************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include <math.h>
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include <stdint.h>

static double squared(double x);
static double ppmdiff(Pnm_ppm pixmap_1, Pnm_ppm pixmap_2,
                      A2Methods_T methods);


/********** main ********
 *
 * Prints the difference between the two PPM images named on the
 * command line
 *
 * Parameters:
 *        int argc:     number of command-line arguments
 *        char *argv[]: the arguments: image1 image2
 *
 * Return: EXIT_SUCCESS on success
 *
 * Expects
 *        exactly two filenames naming readable PPM images
 * Notes:
 *        Exits with EXIT_FAILURE on bad usage or if a file can't be
 *        opened
 *        Will give Runtime Error if a file is not a valid PPM
 ************************/
int main(int argc, char *argv[])
{
        FILE *image_1;
        FILE *image_2;
        A2Methods_T methods = uarray2_methods_plain;
        if ((argv[0] != NULL) && (argc == 3)) {
                image_1 = fopen(argv[1], "r");
                image_2 = fopen(argv[2], "r");
                if (image_1 == NULL || image_2 == NULL) {
                        fprintf(stderr, "Could not open files\n");
                        exit(EXIT_FAILURE);
                }
        } else {
                fprintf(stderr, "Usage: %s image1 image2\n", argv[0]);
                exit(EXIT_FAILURE);
        }
        Pnm_ppm pixmap_1 = Pnm_ppmread(image_1, methods);
        Pnm_ppm pixmap_2 = Pnm_ppmread(image_2, methods);
        double error = ppmdiff(pixmap_1, pixmap_2, methods);
        printf("%.4f\n", error);
        fclose(image_1);
        fclose(image_2);
        Pnm_ppmfree(&pixmap_1);
        Pnm_ppmfree(&pixmap_2);
        exit(EXIT_SUCCESS);
}

/********** ppmdiff ********
 *
 * Computes the root mean square difference between two images
 *
 * Parameters:
 *        Pnm_ppm pixmap_1:    the first image
 *        Pnm_ppm pixmap_2:    the second image
 *        A2Methods_T methods: methods for both images' pixel arrays
 *
 * Return: the difference, from 0.0 (identical) to 1.0
 *
 * Expects
 *        pixmap_1 and pixmap_2 are not NULL
 * Notes:
 *        Only compares pixels inside the smaller width and height
 ************************/
static double ppmdiff(Pnm_ppm pixmap_1, Pnm_ppm pixmap_2,
                      A2Methods_T methods)
{
        double sum = 0;
        A2Methods_UArray2 pixel_1 = pixmap_1->pixels;
        A2Methods_UArray2 pixel_2 = pixmap_2->pixels;
        double denom_1 = pixmap_1->denominator;
        double denom_2 = pixmap_2->denominator;
        int width = (pixmap_1->width < pixmap_2->width) ? pixmap_1->width
                                                        : pixmap_2->width;
        int height = (pixmap_1->height < pixmap_2->height) ? pixmap_1->height
                                                           : pixmap_2->height;
        for (int row_num = 0; row_num < height; row_num++) {
                for (int col_num = 0; col_num < width; col_num++) {
                        struct Pnm_rgb *data_1 = methods->at(pixel_1, col_num,
                                                             row_num);
                        struct Pnm_rgb *data_2 = methods->at(pixel_2, col_num,
                                                             row_num);
                        double red_1 = data_1->red / denom_1;
                        double red_2 = data_2->red / denom_2;
                        double green_1 = data_1->green / denom_1;
                        double green_2 = data_2->green / denom_2;
                        double blue_1 = data_1->blue / denom_1;
                        double blue_2 = data_2->blue / denom_2;
                        double value = squared(red_1 - red_2)
                                       + squared(green_1 - green_2)
                                       + squared(blue_1 - blue_2);
                        sum = sum + value;
                }
        }
        return sqrt(sum / (3.0 * width * height));
}

/********** squared ********
 *
 * Squares a number
 *
 * Parameters:
 *        double x: the number
 *
 * Return: x * x
 ************************/
static double squared(double x)
{
        return x * x;
}
