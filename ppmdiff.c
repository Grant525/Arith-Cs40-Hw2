#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"
#include "uarray2.c"
#include <math.h>
#include "pnm.h"


int squared(int x);
double ppmdiff(Pnm_ppm pixmap_1, Pnm_ppm pixmap_2, A2methods methods);

void main(int argc, char *argv[]){
    if ((argv[0] != NULL)) {
            FILE *image_1 = fopen(argv[1], "r");
            FILE *image_2 = fopen(argv[2], "r");
            if (image_1 == NULL or image_2 == NULL) {
                    fprintf(stderr, "Could not open files");
                    exit(EXIT_FAILURE);
            }
    }    
    Pnm_ppm pixmap_1 = Pnm_ppmread(image_1, methods);
    Pnm_ppm pixmap_2 = Pnm_ppmread(image_2, methods);
    double error = ppmdiff(pixmap_1, pixmap_2, A2methods methods);
    Pnm_ppmwrite(stdout, error);
}


double ppmdiff(Pnm_ppm pixmap_1, Pnm_ppm pixmap_2, A2methods methods){
        int sum = 0;
        pixel_1 = pixmap_1->pixels;
        pixel_2 = pixmap_2->pixels;
        int width = pixel_1->width;
        int height = pixel_1->height;
        for ( int row_num = 0; row_num < height; row_num++) {
                for ( int col_num = 0; col_num < width; col_num++) {
                        *(struct Pnm_rgb*) data_1 = methods->at(pixel_1, col_num, row_num);
                        *(struct Pnm_rgb*) data_2 = methods->at(pixel_2, col_num, row_num);
                        int red_1 =  data_1->red;
                        int red_2 =  data_2->red;
                        int green_1 =  data_1->green;
                        int green_2 =  data_2->green;
                        int blue_1 =  data_1->blue;
                        int blue_2 =  data_2->blue;
                        int value = squared((red_1 - red_2)) + squared((green_1 - green_2)) + squared((blue_1 - blue_2));
                        sum = sum + value;
                }
        }
            return sqrt(sum / (3 * width * height));
}

int squared(int x){
    return x * x;
}
