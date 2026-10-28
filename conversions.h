#ifndef CONVERSIONS_H
#define CONVERSIONS_H

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

A2Methods_UArray2 create_float_pixmap(unsigned width, unsigned height,
                                      A2Methods_T methods);
void Convert_to_float(A2Methods_UArray2 floats, Pnm_ppm pixmap, A2Methods_T methods);

//decompressions
void quanitze_RGB(struct Pnm_rgb *p, double r, double g, double b, int denominator);
struct Pnm_rgb Convert_to_RGB(double y, double pb, double pr, int denominator);


#endif