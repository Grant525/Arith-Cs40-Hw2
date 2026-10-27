#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "arith40.h"
#include "bitpack.h"
#include <math.h>


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
    uint_t code_word = Bitpack_newu(codeword, 9, 23, q.q_a);
    code_word = Bitpack_news(codeword, 5, 18, q.q_b);
    code_word = Bitpack_news(codeword, 5, 13, q.q_c);
    code_word = Bitpack_news(codeword, 5, 8, q.q_d);
    code_word = Bitpack_news(codeword, 4, 4, four_bit_pb);
    code_word = Bitpack_news(codeword, 4, 0, four_bit_pr);
}

struct dequant_abcd dequantize(struct quant_abcd q) {
    struct dequant_abcd f;
    f.a = q.q_a / 511.0;
    f.b = q.q_b / 50.0;
    f.c = q.q_c / 50.0;
    f.d = q.q_d / 50.0;
    return f;
}

void unpack_codeword(A2Methods_UArray2 words) {
    
}