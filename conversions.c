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

struct RGB quanitze_RGB(double r, double g, double b, int denominator);

A2Methods_UArray2 create_float_pixmap(unsigned width, unsigned height, 
        A2Methods_T methods) {
        A2Methods_UArray2 pixmap_new = methods->new(width, height, 
                sizeof(struct pixmap_float));
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

//Convert_to_components() added as well
void Convert_to_float(A2Methods_UArray2 pixmap_new, Pnm_ppm pixmap) { // it is 2d array, i changed variable
        A2Methods_T methods = pixmap->methods;
        int width = pixmap->width; //readability no parenthesis needed
        int height = pixmap->height;
        
        for (int row_num = 0; row_num < height; row_num++) {
                for (int col_num = 0; col_num < width; col_num++) {//loop logic was wrong
                        struct Pnm_rgb *rgb = methods->at(pixmap->pixels, 
                                col_num, row_num);
                        double r = (double)rgb->red / pixmap->denominator; // formula expect 0 to 1 so i divided 
                        double b = (double)rgb->blue / pixmap->denominator; // these needed to be double
                        double g = (double)rgb->green / pixmap->denominator;
                        
                        double y = 0.299 * r + 0.587 * g + 0.114 * b;
                        double pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
                        double pr = 0.5 * r - 0.418688 * g - 0.081312 * b; 
                        
                        struct pixmap_float *pf = methods->at(pixmap_new, 
                                col_num, row_num); //about to convert it to float through methods
                    pf->y = y;
                    pf->pb = pb;
                    pf->pr = pr;
                }
        }
        //this is a void function so we dont return anything, we are just converting 
        //the one we already made
}

void Convert_to_RGB(UarrayT pixmap_new, UarrayT pixmap){
        int width = methods->width(pixmap)  
        int height = methods->height(pixmap)  
        
        for (int row_num, row_num < height, row_num++){
                for (int col_num, col_num < width, col_num++){
                    int y = pixmap->at(col_num, row_num)->y
                    int pb = pixmap->at(col_num, row_num)->pb
                    int pr = pixmap->at(col_num, row_num)->pr
                    
                    int r = 1.0 * y + 0.0 * pb + 1.402 * pr;
                    int g = 1.0 * y - 0.344136 * pb - 0.714136 * pr;
                    int b = 1.0 * y + 1.772 * pb + 0.0 * pr;

                    struct RGB {
                        unsigned r = red;
                        unsigned b = blue;
                        unsigned g = green
                    }
                    struct RGB data = quanitze_RGB(r,g,b, pixmap->denominator);
        
                    pixmap_new->at(col_num, row_num)->red = data.red;
                    pixmap_new->at(col_num, row_num)->blue = data.blue;
                    pixmap_new->at(col_num, row_num)->green = data.green;
                }
        }
}

struct RGB quanitze_RGB(double r, double g, double b, int denominator){
    if (r < 0) r = 0;
    if (b < 0) b = 0;
    if (g < 0) g = 0;
    if (r > 1) r = 1;
    if (b > 1) b = 1;
    if (g > 1) g = 1;
    struct RGB data;
    data.red = (unsigned) round(r * denominator);
    data.blue = (unsigned) round(b * denominator);
    data.green = (unsigned) round(g * denominator);
    
    return data
} 
                    



