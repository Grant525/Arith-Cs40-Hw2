/**************************************************************
 *
 *                     bitpack.c
 *
 *        Assignment: CS 40 HW 4, Arith
 *        Authors:    Micheal Gonzales (mgonza20),
 *                    Grant Caile (gcaile01)
 *        Date:       October 28, 2026
 *
 *        Implementation of the Bitpack interface (bitpack.h):
 *        checking whether integers fit in a given number of bits,
 *        and reading and writing signed and unsigned bit fields
 *        inside a 64-bit word. Fields are described by a width and
 *        the position of their least significant bit (lsb).
 *
 **************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bitpack.h"
#include "except.h"
#include "assert.h"

const Except_T Bitpack_Overflow = { "Overflow packing bits" };

/********** Bitpack_fitsu ********
 *
 * Tells whether an unsigned integer can be represented in width bits
 *
 * Parameters:
 *        uint64_t n:     the value to check
 *        unsigned width: number of bits available
 *
 * Return: true if n fits in width unsigned bits, false if not
 *
 * Expects
 *        width <= 64
 * Notes:
 *        Will give Runtime Error if width > 64
 ************************/
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width <= 64);
        if (width == 64) {
                return true;
        }
        uint64_t limit = ((uint64_t)1 << width) - 1;
        return (n <= limit);
}

/********** Bitpack_fitss ********
 *
 * Tells whether a signed integer can be represented in width bits of
 * two's complement
 *
 * Parameters:
 *        int64_t n:      the value to check
 *        unsigned width: number of bits available
 *
 * Return: true if n fits in width signed bits, false if not
 *
 * Expects
 *        width <= 64
 * Notes:
 *        Only 0 fits in a width of 0
 *        Will give Runtime Error if width > 64
 ************************/
bool Bitpack_fitss(int64_t n, unsigned width)
{
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

/********** Bitpack_getu ********
 *
 * Extracts an unsigned field from a word
 *
 * Parameters:
 *        uint64_t word:  the word to read from
 *        unsigned width: width of the field in bits
 *        unsigned lsb:   position of the field's least significant bit
 *
 * Return: the field's value as an unsigned integer
 *
 * Expects
 *        width + lsb <= 64
 * Notes:
 *        A field of width 0 has value 0
 *        Will give Runtime Error if width + lsb > 64
 ************************/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
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

/********** Bitpack_gets ********
 *
 * Extracts a signed (two's complement) field from a word
 *
 * Parameters:
 *        uint64_t word:  the word to read from
 *        unsigned width: width of the field in bits
 *        unsigned lsb:   position of the field's least significant bit
 *
 * Return: the field's value, sign-extended to 64 bits
 *
 * Expects
 *        width + lsb <= 64
 * Notes:
 *        A field of width 0 has value 0
 *        Will give Runtime Error if width + lsb > 64
 ************************/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert((width + lsb) <= 64);
        if (width == 64) {
                return word;
        }
        if (width == 0) {
                return 0;
        }
        word = word >> lsb;
        uint64_t sign_bit = (uint64_t)1 << (width - 1);
        if ((sign_bit & word) != 0) {
                uint64_t mask = ~(((uint64_t)1 << width) - 1);
                word = word | mask;
                return word;
        } else {
                uint64_t mask = ((uint64_t)1 << width) - 1;
                word = word & mask;
                return word;
        }
}

/********** Bitpack_newu ********
 *
 * Returns a copy of word with one field replaced by an unsigned value
 *
 * Parameters:
 *        uint64_t word:  the original word
 *        unsigned width: width of the field in bits
 *        unsigned lsb:   position of the field's least significant bit
 *        uint64_t value: the new value for the field
 *
 * Return: the new word; bits outside the field are unchanged
 *
 * Expects
 *        width <= 64 and width + lsb <= 64
 *        value fits in width unsigned bits
 * Notes:
 *        Will give Runtime Error if width or width + lsb is greater than 64
 *        Raises Bitpack_Overflow if value does not fit in width bits
 ************************/
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!Bitpack_fitsu(value, width)) {
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

/********** Bitpack_news ********
 *
 * Returns a copy of word with one field replaced by a signed value
 *
 * Parameters:
 *        uint64_t word:  the original word
 *        unsigned width: width of the field in bits
 *        unsigned lsb:   position of the field's least significant bit
 *        int64_t value:  the new value for the field
 *
 * Return: the new word; bits outside the field are unchanged
 *
 * Expects
 *        width <= 64 and width + lsb <= 64
 *        value fits in width signed bits
 * Notes:
 *        Will give Runtime Error if width or width + lsb is greater than 64
 *        Raises Bitpack_Overflow if value does not fit in width bits
 ************************/
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                      int64_t value)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!Bitpack_fitss(value, width)) {
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
