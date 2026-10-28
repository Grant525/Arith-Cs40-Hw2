#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bitpack.h"
#include "except.h"
#include "assert.h"

const Except_T Bitpack_Overflow = { "Overflow packing bits" };

//to implement
bool Bitpack_fitsu(uint64_t n, unsigned width) {
        assert(width <= 64);
        if (width == 64) {
                return true;
        }
        uint64_t limit = ((uint64_t)1 << width) - 1;
        return (n <= limit);
}

bool Bitpack_fitss(int64_t n, unsigned width) {
        assert(width <= 64);
        if (width == 64) {
                return true;
        }
        if (width == 0) {
                return n == 0;
        }
        int64_t limit = ((int64_t)1 << (width - 1)) - 1;
        int64_t n_limit = -((int64_t)1 << (width - 1));
        return ((n <= limit) && (n >= n_limit));
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) {
        assert((width + lsb) <= 64);
        if (width == 64) {
                return word;
        }
        if (width == 0) {
                return 0;
        }
        word = word >> lsb;
        uint64_t mask = ((uint64_t)1 << width) - 1;
        word = word & mask;
        return word;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) {
        assert((width + lsb) <= 64);
        if (width == 64) {
                return word;
        }
        if (width == 0) {
                return 0;
        }
        word = word >> lsb;
        uint64_t sign_bit = (uint64_t)1 << (width - 1);
        if ((sign_bit & word) != 0){
                uint64_t mask =  ~(((uint64_t)1 << width) - 1);
                word = word | mask;
                return word;
        }
        else {
                uint64_t mask = ((uint64_t)1 << width) - 1;
                word = word & mask;
                return word;
        }

}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value) {
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!Bitpack_fitsu(value, width)){
                RAISE(Bitpack_Overflow);
        }
        if (width == 64) {
                return value;
        }
        if (width == 0) {
                return word;
        }
        uint64_t below = (((uint64_t)1 << lsb) - 1);
        uint64_t above;
        if (width + lsb == 64) {
                above = 0;
        } else {
                above = ~(((uint64_t)1 << (width + lsb)) - 1);
        }
        uint64_t mask = below ^ above;
        word = word & mask;
        uint64_t v_mask = value << lsb;
        word = word | v_mask;
        return word;
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, int64_t value) {
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!Bitpack_fitss(value, width)){
                RAISE(Bitpack_Overflow);
        }
         if (width == 64) {
                return (uint64_t)value;
        }
        if (width == 0) {
                return word;
        }
        uint64_t width_mask = ((uint64_t)1 << width) - 1;
        uint64_t trimmed = (uint64_t)value & width_mask;

        return Bitpack_newu(word, width, lsb, trimmed);
}
