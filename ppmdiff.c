#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include <math.h>
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include <stdint.h>

double squared(double x);
double ppmdiff(Pnm_ppm pixmap_1, Pnm_ppm pixmap_2, A2Methods_T methods);


int main(int argc, char *argv[]){
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
    }    
    else {
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


double ppmdiff(Pnm_ppm pixmap_1, Pnm_ppm pixmap_2, A2Methods_T methods){
        double sum = 0;
        A2Methods_UArray2 pixel_1 = pixmap_1->pixels;
        A2Methods_UArray2 pixel_2 = pixmap_2->pixels;
        int width = ((pixmap_1->width) < (pixmap_2->width) ? pixmap_1->width : pixmap_2->width);
        int height = ((pixmap_1->height) < (pixmap_2->height) ? pixmap_1->height: pixmap_2->height);
        for ( int row_num = 0; row_num < height; row_num++) {
                for ( int col_num = 0; col_num < width; col_num++) {
                        struct Pnm_rgb *data_1 = methods->at(pixel_1, col_num, row_num);
                        struct Pnm_rgb *data_2 = methods->at(pixel_2, col_num, row_num);
                        double red_1 =  (double)data_1->red / pixmap_1->denominator;
                        double red_2 =  (double)data_2->red / pixmap_2->denominator;
                        double green_1 =  (double)data_1->green/ pixmap_1->denominator;
                        double green_2 =  (double)data_2->green/ pixmap_2->denominator;
                        double blue_1 =  (double)data_1->blue/ pixmap_1->denominator;
                        double blue_2 =  (double)data_2->blue/ pixmap_2->denominator;
                        double value = squared((red_1 - red_2)) + squared((green_1 - green_2)) + squared((blue_1 - blue_2));
                        sum = sum + value;
                }
        }
        return sqrt(sum / (3.0 * width * height));
}

double squared(double x){
    return x * x;
}
