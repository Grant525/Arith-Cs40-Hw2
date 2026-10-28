/**************************************************************
 *
 *                     conversions.h
 *
 *        Assignment: CS 40 HW 4, Arith
 *        Authors:    Micheal Gonzales (mgonza20),
 *                    Grant Caile (gcaile01)
 *        Date:       October 28, 2026
 *
 *        Interface for converting pixels between scaled-integer RGB
 *        and floating-point component video (Y, Pb, Pr).
 *
 **************************************************************/
#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <pnm.h>
#include <a2methods.h>

/* One pixel in component video color space */
struct pixmap_float {
        double y;       /* luma, 0 to 1 */
        double pb;      /* blue-difference chroma, -0.5 to 0.5 */
        double pr;      /* red-difference chroma, -0.5 to 0.5 */
};


A2Methods_UArray2 create_float_pixmap(unsigned width, unsigned height,
                                      A2Methods_T methods);
void Convert_to_float(A2Methods_UArray2 floats, Pnm_ppm pixmap,
                      A2Methods_T methods);

/* decompressions */
void quanitze_RGB(struct Pnm_rgb *p, double r, double g, double b,
                  int denominator);
struct Pnm_rgb Convert_to_RGB(double y, double pb, double pr, int denominator);


#endif
