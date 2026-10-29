/**************************************************************
 *
 *                     block.c
 *
 *        Assignment: CS 40 HW 4, Arith
 *        Authors:    Michael Gonzales (mgonza20),
 *                    Grant Caile (gcaile01)
 *        Date:       October 28, 2026
 *
 *        Implementation of block.h. During compression,
 *        chroma_conversion is mapped over a blocked array with
 *        blocksize 2, so it sees the four pixels of each 2x2 block
 *        in a row. Once a block is complete it computes the chroma
 *        sums and the DCT and hands the block to pack_codeword.
 *        inv_DCT reverses the transform when
 *        decompressing.
 *
 **************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"
#include "uarray2.h"
#include <stdint.h>
#include "block.h"
#include "codeword.h"
#include "images.h"

/********** DCT ********
 *
 * Computes the discrete cosine transform of a block's four luma values
 *
 * Parameters:
 *        double y1, y2, y3, y4: luma of the top-left, top-right,
 *                               bottom-left, and bottom-right pixels
 *        void *cl:              pointer to a struct pixmap_block
 *
 * Return: none
 *
 * Expects
 *        cl is not NULL
 * Notes:
 *        Stores a, b, c, d in cos_coef[0] through cos_coef[3] of cl
 ************************/
void DCT(double y1, double y2, double y3, double y4, void *cl)
{
        struct pixmap_block *block = cl;
        block->cos_coef[0] = (y4 + y3 + y2 + y1) / 4.0;
        block->cos_coef[1] = (y4 + y3 - y2 - y1) / 4.0;
        block->cos_coef[2] = (y4 - y3 + y2 - y1) / 4.0;
        block->cos_coef[3] = (y4 - y3 - y2 + y1) / 4.0;
}

/********** chroma_conversion ********
 *
 * Apply function for a blocked map with blocksize 2. Gathers the four
 * pixels of each 2x2 block, then computes and writes its codeword.
 *
 * Parameters:
 *        int col, int row:         position of the pixel (unused)
 *        A2Methods_UArray2 pixmap: the array being mapped (unused)
 *        void *elem:               pointer to a struct pixmap_float
 *        void *cl:                 pointer to a struct pixmap_block
 *                                  whose count starts at 0
 *
 * Return: none
 *
 * Expects
 *        elem and cl are not NULL
 *        the map visits all four pixels of a block before the next block
 * Notes:
 *        Every fourth call writes one codeword to stdout and resets count
 ************************/
void chroma_conversion(int col, int row, A2Methods_UArray2 pixmap, void *elem,
                       void *cl)
{
        (void)col;
        (void)row;
        (void)pixmap;
        struct pixmap_block *block = cl;
        block->pa[block->count] = *(struct pixmap_float *)elem;
        block->count++;
        if (block->count == 4) {
                block->pb_sum = block->pa[0].pb + block->pa[1].pb
                                + block->pa[2].pb + block->pa[3].pb;
                block->pr_sum = block->pa[0].pr + block->pa[1].pr
                                + block->pa[2].pr + block->pa[3].pr;
                DCT(block->pa[0].y, block->pa[1].y,
                    block->pa[2].y, block->pa[3].y, cl);
                pack_codeword(cl);
                block->count = 0;
        }
}

/********** inv_DCT ********
 *
 * Recovers a block's four luma values from its DCT coefficients
 *
 * Parameters:
 *        double a, b, c, d: the DCT coefficients
 *        void *cl:          pointer to a struct pixmap_block_d
 *
 * Return: none
 *
 * Expects
 *        cl is not NULL
 * Notes:
 *        Stores Y1 through Y4 in y[0] through y[3] of cl
 ************************/
void inv_DCT(double a, double b, double c, double d, void *cl)
{
        struct pixmap_block_d *block = cl;
        block->y[0] = a - b - c + d;
        block->y[1] = a - b + c - d;
        block->y[2] = a + b - c - d;
        block->y[3] = a + b + c + d;
}
