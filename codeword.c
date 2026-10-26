
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "Uarray2.c"
#include <math.h>




pack_codeword(cl){
    struct pixmap_block *block = cl;
    int four_bit_pb = Arith40_index_of_chroma(block->pb_sum / 4.0);
    int four_bit_pr = Arith40_index_of_chroma(block->pr_sum / 4.0);
    quantize(*cl)
}

quantize(cl){
    
}
