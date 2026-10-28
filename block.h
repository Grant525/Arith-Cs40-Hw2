#ifndef BLOCK_H
#define BLOCK_H

#include <a2methods.h>
#include "conversions.h"

struct pixmap_block {
    int count;
    struct pixmap_float pa[4];
    double cos_coef[4];
    double pb_sum;
    double pr_sum;
};

struct pixmap_block_d {
    int count;
    struct pixmap_float pa[4];
    double y[4];
    double pb;
    double pr;
};

void DCT(double y1, double y2, double y3, double y4, void *cl);

void chroma_conversion(int col, int row, A2Methods_UArray2 pixmap, void *elem,
                       void *cl);

void inv_DCT(double a, double b, double c, double d, void *cl);

#endif
