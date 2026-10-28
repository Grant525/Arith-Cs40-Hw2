/**************************************************************
 *
 *                     codeword.h
 *
 *        Assignment: CS 40 HW 4, Arith
 *        Authors:    Micheal Gonzales (mgonza20),
 *                    Grant Caile (gcaile01)
 *        Date:       October 28, 2026
 *
 *        Interface for packing a 2x2 block into a 32-bit codeword
 *        during compression, and unpacking codewords back into
 *        pixels during decompression.
 *
 **************************************************************/
#ifndef CODEWORD_H
#define CODEWORD_H

#include <pnm.h>
#include <a2methods.h>

double clamp(double x);

void pack_codeword(void *cl);

void unpack_codeword(Pnm_ppm pixmap, A2Methods_UArray2 words,
                     A2Methods_T methods);

#endif
