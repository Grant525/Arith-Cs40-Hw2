#ifndef CODEWORD_H
#define CODEWORD_H

#include <pnm.h>
#include <a2methods.h>

double clamp(double x);

void pack_codeword(void *cl);

void unpack_codeword(Pnm_ppm pixmap, A2Methods_UArray2 words,
                     A2Methods_T methods);

#endif
