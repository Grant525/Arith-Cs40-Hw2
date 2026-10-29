/**************************************************************
 *
 *                     block.h
 *
 *        Assignment: CS 40 HW 4, Arith
 *        Authors:    Michael Gonzales (mgonza20),
 *                    Grant Caile (gcaile01)
 *        Date:       October 28, 2026
 *
 *        Interface for working with 2x2 blocks of pixels: the
 *        discrete cosine transform (and its inverse) on the four
 *        luma values, and the apply function that gathers each
 *        block during compression.
 *
 **************************************************************/
#ifndef BLOCK_H
#define BLOCK_H

#include <a2methods.h>
#include "conversions.h"

/*
 * Closure used while compressing. Holds the pixels of the 2x2 block
 * currently being gathered and the values computed from them.
 */
struct pixmap_block {
        int count;                  /* pixels gathered so far (0-4) */
        struct pixmap_float pa[4];  /* the block's pixels, row-major */
        double cos_coef[4];         /* DCT coefficients a, b, c, d */
        double pb_sum;              /* sum of the four Pb values */
        double pr_sum;              /* sum of the four Pr values */
};

/*
 * One 2x2 block while decompressing: the four luma values recovered
 * by the inverse DCT and the block's average chroma.
 */
struct pixmap_block_d {
        int count;
        struct pixmap_float pa[4];
        double y[4];                /* Y1-Y4, row-major in the block */
        double pb;
        double pr;
};

void DCT(double y1, double y2, double y3, double y4, void *cl);

void chroma_conversion(int col, int row, A2Methods_UArray2 pixmap, void *elem,
                       void *cl);

void inv_DCT(double a, double b, double c, double d, void *cl);

#endif
