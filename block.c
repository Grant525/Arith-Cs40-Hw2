#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"
#include "Uarray2.c"


struct pixmap_block {
    int count;
    struct pixmap_float pa[4];
    double cos_coef[4]
    double pb_sum;
    double pr_sum;
};

struct pixmap_block_d {
    int count;
    struct pixmap_float pa[4];
    double y[4]
    double pb_sum;
    double pr_sum;
};


A2Methods_T methods = uarray2_methods_blocked;

void chroma_conversion(int col, int row, A2Methods_UArray2 pixmap, void *elem, void *cl){
        (void)col; (void)row; (void)pixmap;
        struct pixmap_block *block = cl;
        block->pa[block->count] = *(struct pixmap_float *)elem;
        block->count++;
        if (block->count == 4){
            block->pb_sum = block->pa[0].pb + block->pa[1].pb + block->pa[2].pb + block->pa[3].pb;
            block->pr_sum = block->pa[0].pr + block->pa[1].pr + block->pa[2].pr + block->pa[3].pr;
            DCT(block->pa[0].y,block->pa[1].y,block->pa[2].y,block->pa[3].y, cl);
            codeword = pack_codeword(cl);
            write_codeword(codeword);
            block->count = 0;
        }
}

void DCT(double y1, double y2, double y3, double y4, void *cl) {
        struct pixmap_block *block = cl;
        block->cos_ceof[0] = (y4 + y3 + y2 + y1) / 4.0;
        block->cos_ceof[1] = (y4 + y3 - y2 - y1) / 4.0;
        block->cos_ceof[2] = (y4 - y3 + y2 - y1) / 4.0;
        block->cos_ceof[3] = (y4 - y3 - y2 + y1) / 4.0;

}



inv_DCT(double a, double b, double c, double d, void *cl){
        struct pixmap_block_d *block = cl;
        block->y[0] = a - b - c + d;
        block->y[1] = a - b + c - d;
        block->y[2] = a + b - c - d;
        block->y[3] = a + b + c + d;
}

create_float_pixmap() 