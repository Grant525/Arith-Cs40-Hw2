/**************************************************************
 *
 *                     codeword.c
 *
 *        Assignment: CS 40 HW 4, Arith
 *        Authors:    Michael Gonzales (mgonza20),
 *                    Grant Caile (gcaile01)
 *        Date:       October 28, 2026
 *
 *        Implementation of codeword.h. Each 2x2 block becomes one
 *        32-bit codeword laid out as:
 *
 *            a: 9 bits unsigned at lsb 23
 *            b, c, d: 5 bits signed at lsb 18, 13, 8
 *            index(avg Pb): 4 bits unsigned at lsb 4
 *            index(avg Pr): 4 bits unsigned at lsb 0
 *
 *        b, c, and d are clamped to [-0.3, 0.3] before being
 *        quantized. Uses bitpack.h for the fields and arith40.h for
 *        the chroma indices.
 *
 **************************************************************/

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

/* Quantized DCT coefficients of one block */
struct quant_abcd {
        unsigned q_a;
        int q_b;
        int q_c;
        int q_d;
};

/********** clamp ********
 *
 * Limits a DCT coefficient to the range [-0.3, 0.3]
 *
 * Parameters:
 *        double x: the coefficient
 *
 * Return: x, or the nearest end of the range if x is outside it
 ************************/
double clamp(double x)
{
        if (x > 0.3) {
                return 0.3;
        }
        if (x < -0.3) {
                return -0.3;
        } else {
                return x;
        }
}

/********** quantize ********
 *
 * Quantizes a block's DCT coefficients to the integers stored in the
 * codeword
 *
 * Parameters:
 *        void *cl: pointer to a struct pixmap_block whose cos_coef is set
 *
 * Return: a scaled to 0-511, and b, c, d clamped and scaled to -15..15
 *
 * Expects
 *        cl is not NULL
 ************************/
struct quant_abcd quantize(void *cl)
{
        struct quant_abcd q;
        struct pixmap_block *block = cl;
        q.q_a = (unsigned)round(block->cos_coef[0] * 511);
        q.q_b = round(clamp(block->cos_coef[1]) * 50);
        q.q_c = round(clamp(block->cos_coef[2]) * 50);
        q.q_d = round(clamp(block->cos_coef[3]) * 50);
        return q;
}

/********** pack_codeword ********
 *
 * Packs one block into a codeword and writes it to stdout
 *
 * Parameters:
 *        void *cl: pointer to a struct pixmap_block whose cos_coef,
 *                  pb_sum, and pr_sum are set
 *
 * Return: none
 *
 * Expects
 *        cl is not NULL
 * Notes:
 *        Writes 4 bytes to stdout, most significant byte first
 *        Raises Bitpack_Overflow if a value does not fit its field
 ************************/
void pack_codeword(void *cl)
{
        struct pixmap_block *block = cl;
        unsigned four_bit_pb = Arith40_index_of_chroma(block->pb_sum / 4.0);
        unsigned four_bit_pr = Arith40_index_of_chroma(block->pr_sum / 4.0);
        struct quant_abcd q = quantize(cl);
        uint64_t code_word = Bitpack_newu(0, 9, 23, q.q_a);
        code_word = Bitpack_news(code_word, 5, 18, q.q_b);
        code_word = Bitpack_news(code_word, 5, 13, q.q_c);
        code_word = Bitpack_news(code_word, 5, 8, q.q_d);
        code_word = Bitpack_newu(code_word, 4, 4, four_bit_pb);
        code_word = Bitpack_newu(code_word, 4, 0, four_bit_pr);
        write_codeword(code_word);
}

/********** unpack_word ********
 *
 * Unpacks one codeword and writes its four pixels into the pixmap
 *
 * Parameters:
 *        uint64_t word:       the codeword
 *        Pnm_ppm pixmap:      the image being rebuilt
 *        A2Methods_T methods: methods for pixmap's pixel array
 *        unsigned column:     column of the block (in blocks)
 *        unsigned row:        row of the block (in blocks)
 *
 * Return: none
 *
 * Expects
 *        pixmap and methods are not NULL
 *        the block lies inside the pixmap
 ************************/
static void unpack_word(uint64_t word, Pnm_ppm pixmap, A2Methods_T methods,
                        unsigned column, unsigned row)
{
        double a = Bitpack_getu(word, 9, 23) / 511.0;
        double b = Bitpack_gets(word, 5, 18) / 50.0;
        double c = Bitpack_gets(word, 5, 13) / 50.0;
        double d = Bitpack_gets(word, 5, 8) / 50.0;
        unsigned four_bit_pb = Bitpack_getu(word, 4, 4);
        unsigned four_bit_pr = Bitpack_getu(word, 4, 0);
        double pb = Arith40_chroma_of_index(four_bit_pb);
        double pr = Arith40_chroma_of_index(four_bit_pr);
        struct pixmap_block_d block;
        inv_DCT(a, b, c, d, &block);
        block.pb = pb;
        block.pr = pr;
        for (int i = 0; i < 4; i++) {
                struct Pnm_rgb *pix = methods->at(pixmap->pixels,
                                                  2 * column + i % 2,
                                                  2 * row + i / 2);
                *pix = Convert_to_RGB(block.y[i], pb, pr,
                                      pixmap->denominator);
        }
}

/********** unpack_codeword ********
 *
 * Fills a pixmap's pixels from an array of codewords
 *
 * Parameters:
 *        Pnm_ppm pixmap:          the image to fill
 *        A2Methods_UArray2 words: one uint64_t codeword per 2x2 block
 *        A2Methods_T methods:     methods for both words and pixmap
 *
 * Return: none
 *
 * Expects
 *        words is not NULL
 *        words is (width / 2) by (height / 2) of the pixmap
 * Notes:
 *        Will give Runtime Error if words is NULL or its size does not match pixmap
 ************************/
void unpack_codeword(Pnm_ppm pixmap, A2Methods_UArray2 words,
                     A2Methods_T methods)
{
        assert(words != NULL);
        unsigned count = methods->width(words) * methods->height(words);
        assert(count == (pixmap->width / 2) * (pixmap->height / 2));
        for (unsigned row = 0; row < pixmap->height / 2; row++) {
                for (unsigned column = 0; column < pixmap->width / 2;
                     column++) {
                        uint64_t word = *(uint64_t *)methods->at(words,
                                                                 column, row);
                        unpack_word(word, pixmap, methods, column, row);
                }
        }
}
