#ifndef CONVERSIONS_H
#define CONVERSEIONS_H

#include <pnm.h>
#include <a2methods.h>

struct pixmap_float {
    double y;
    double pb;
    double pr;
};

struct RGB {
    unsigned red;
    unsigned green;
    unsigned blue;
};
//didnt finish but i started this file
//compression
A2Methods_UArray2 create_float_pixmap(unsigned width, unsigned height);
void Convert_to_float(A2Methods_UArray2 floats, Pnm_ppm pixmap);

//decompressions
struct RGB quantize_RGB(double r, double g, double b, int denominator);
void Convert_to_RGB(UarrayT pixmap_new, UarrayT pixmap);

#endif