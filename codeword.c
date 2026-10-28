#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "arith40.h"
#include "bitpack.h"
#include "a2methods.h"
#include "pnm.h"
#include <math.h>
#include <stdint.h>
#include "codeword.h"
#include "block.h"
#include "conversions.h"
#include "images.h"


struct quant_abcd {
    unsigned q_a;
    int q_b;
    int q_c;
    int q_d;
};

struct dequant_abcd {
    double a, b, c, d;
};

double clamp(double x) {
    if (x > 0.3) {
        return 0.3;
    }
    if (x < -0.3) {
        return -0.3;
    }
    else {
        return x;
    }
}

struct quant_abcd quantize(void *cl){
        struct quant_abcd q; 
        struct pixmap_block *block = cl;
        q.q_a = (unsigned)round(block->cos_coef[0] * 511);
        q.q_b = round(clamp(block->cos_coef[1]) * 50);
        q.q_c = round(clamp(block->cos_coef[2]) * 50);
        q.q_d = round(clamp(block->cos_coef[3]) * 50);
        return q;
}

void pack_codeword(void *cl){
    struct pixmap_block *block = cl;
    unsigned four_bit_pb = Arith40_index_of_chroma(block->pb_sum / 4.0);
    unsigned four_bit_pr = Arith40_index_of_chroma(block->pr_sum / 4.0);
    struct quant_abcd q = quantize(cl);
    uint32_t code_word = Bitpack_newu(0, 9, 23, q.q_a);
    code_word = Bitpack_news(code_word, 5, 18, q.q_b);
    code_word = Bitpack_news(code_word, 5, 13, q.q_c);
    code_word = Bitpack_news(code_word, 5, 8, q.q_d);
    code_word = Bitpack_newu(code_word, 4, 4, four_bit_pb);
    code_word = Bitpack_newu(code_word, 4, 0, four_bit_pr);
    write_codeword(code_word);
}

struct dequant_abcd dequantize(struct quant_abcd q) {
    struct dequant_abcd f;
    f.a = q.q_a / 511.0;
    f.b = q.q_b / 50.0;
    f.c = q.q_c / 50.0;
    f.d = q.q_d / 50.0;
    return f;
}

void unpack_codeword(Pnm_ppm pixmap, A2Methods_UArray2 words, A2Methods_T methods){
        assert(words != NULL);
        unsigned count = methods->width(words) * methods->height(words);
        assert(count == (pixmap->width / 2) * (pixmap->height / 2));
        for (unsigned row = 0; row < pixmap->height/2; row++) {
                for (unsigned column = 0; column < pixmap->width/2; column++) {
                        uint64_t word = *(uint64_t *)methods->at(words, column, row);
                        double a = Bitpack_getu(word, 9, 23) / 511.0;
                        double b = Bitpack_gets(word, 5, 18) / 50.0;
                        double c = Bitpack_gets(word, 5, 13) / 50.0;
                        double d = Bitpack_gets(word, 5, 8) / 50.0;
                        unsigned four_bit_pb = Bitpack_getu(word, 4, 4);
                        unsigned four_bit_pr = Bitpack_getu(word, 4, 0);
                        double pb =  Arith40_chroma_of_index(four_bit_pb);
                        double pr =  Arith40_chroma_of_index(four_bit_pr);
                        struct pixmap_block_d block;
                        inv_DCT(a, b, c, d, &block);
                        block.pb = pb;
                        block.pr = pr;
                        for (int i = 0; i < 4; i++) {
                                struct Pnm_rgb *pix = methods->at(pixmap->pixels,
                                2 * column + i % 2,
                                2 * row + i / 2);
                                *pix = Convert_to_RGB(block.y[i], pb, pr, pixmap->denominator);
                        }
                }
        }
}