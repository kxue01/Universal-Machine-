/*
 * bitpack.c
 *
 * Implementation of functions to bit-pack
 * data to and from 64-bit unsigned integers.
 *
 * Comp 40 HW4: Arith
 * Fall 2019
 *
 * Authors: Charles Oddleifson (coddle01)
 *          William Varteresian (wvarte01)
 *
 * Last modified: 10_20_2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <bitpack.h>

static uint64_t shift_left(uint64_t word, unsigned shift);
static uint64_t shift_right(uint64_t word, unsigned shift);

/*
 * Checks whether an unsigned value fits in a field of a given width.
 * Input:   - a uin64_t representing a value to be checked
 *          - an unsigned representing the width of a field in bits
 * Returns: - true if the value will fit
 *          - false otherwise
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        if (shift_right(n, width) == 0) {
                return true;
        } else {
                return false;
        }
}

/*
 * Checks whether a signed value fits in a field of a given width.
 * Input:   - an in64_t representing a value to be checked
 *          - an unsigned representing the width of a field in bits
 * Returns: - true if the value will fit
 *          - false otherwise
 */
bool Bitpack_fitss( int64_t n, unsigned width)
{
        int64_t max = (int64_t)shift_left(1, width - 1);
        if (n <= max && n > (max * -1) - 1) {
                return true;
        } else {
                return false;
        }
}

/*
 * Gets the unsigned value of a given bit field in a 64-bit word.
 * Input:   - a uint64_t representing the word
 *          - an unsigned representing the width
 *            of the field to be extracted in bits
 *          - an unsigned representing the position of the field's
 *            least significant bit
 *          It is a checked runtime error for width to be greater than
 *          64 or for width + lsb to be greater than 64.
 * Returns: - a uint64_t representing the value in the given field
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (width == 0) {
                return 0;
        }
        uint64_t mask = ~0;
        mask = shift_right(shift_left(mask, 64 - width), 64 - width - lsb);
        word = word & mask;
        word = shift_right(word, lsb);
        return word;
}

/*
 * Gets the signed value of a given bit field in a 64-bit word.
 * Input:   - a uint64_t representing the word
 *          - an unsigned representing the width
 *            of the field to be extracted in bits
 *          - an unsigned representing the position of the field's
 *            least significant bit
 *          It is a checked runtime error for width to be greater than
 *          64 or for width + lsb to be greater than 64.
 * Returns: - an int64_t representing the value in the given field
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (width == 0) {
                return 0;
        }
        uint64_t mask = ~0;
        mask = shift_right(shift_left(mask, 64 - width), 64 - width - lsb);
        word = word & mask;
        word = shift_right(word, lsb);
        if (shift_right(word, width - 1) != 0) {
                mask = ~0;
                mask = shift_left(mask, width);
                word = word | mask;
        }
        return (int64_t)word;
}

/*
 * Updates the unsigned value in a given bit field of a 64-bit word.
 * Input:   - a uint64_t representing the word
 *          - an unsigned representing the width of the field in bits
 *          - an unsigned representing the position of the field's
 *            least significant bit
 *          - a uint64_t representing the new value to be inserted
 *          It is a checked runtime error for width to be > 64,
 *          for width + lsb to be > 64, or for value not to fit
 *          in a field of width bits.
 * Returns: - a uint64_t representing the updated word
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width,
                      unsigned lsb, uint64_t value)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!Bitpack_fitsu(value, width)) {
                RAISE(Bitpack_Overflow);
        }
        uint64_t mask = ~0;
        mask = shift_right(shift_left(mask, 64 - width), 64 - width - lsb);
        mask = ~mask;
        word = word & mask;
        word = word | shift_left(value, lsb);
        return word;
}

/*
 * Updates the signed value in a given bit field of a 64-bit word.
 * Input:   - a uint64_t representing the word
 *          - an unsigned representing the width of the field in bits
 *          - an unsigned representing the position of the field's
 *            least significant bit
 *          - an int64_t representing the new value to be inserted
 *          It is a checked runtime error for width to be > 64,
 *          for width + lsb to be > 64, or for value not to fit
 *          in a field of width bits.
 * Returns: - a uint64_t representing the updated word
 */
uint64_t Bitpack_news(uint64_t word, unsigned width,
                      unsigned lsb, int64_t value)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }
        uint64_t mask = ~0;
        mask = shift_right(shift_left(mask, 64 - width), 64 - width - lsb);
        mask = ~mask;
        word = word & mask;
        mask = ~0;
        mask = shift_left(mask, width);
        mask = (~mask) & value;
        word = word | shift_left(mask, lsb);
        return word;
}

/*
 * Shifts a 64-bit word left.
 * Input:   - a uint64_t representing the word
 *          - an unsigned representing the number of bits
 *            to shift left
 * Returns: - the word shifted left shift bits
 *          - 0 if shift > 64
 */
static uint64_t shift_left(uint64_t word, unsigned shift)
{
        if (shift < 64) {
                return (word << shift);
        } else {
                return 0;
        }
}

/*
 * Shifts a 64-bit word right.
 * Input:   - a uint64_t representing the word
 *          - an unsigned representing the number of bits
 *            to shift right
 * Returns: - the word shifted rightt shift bits
 *          - 0 if shift > 64
 */
static uint64_t shift_right(uint64_t word, unsigned shift)
{
        if (shift < 64) {
                return (word >> shift);
        } else {
                return 0;
        }
}

/* Exception raised when attempting to insert a value
   into a bit field that is too small to contain it */
Except_T Bitpack_Overflow = { "Overflow packing bits" };
