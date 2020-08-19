/*
 * memarray.c
 * Comp 40 HW6: UM
 * Fall 2019
 *
 * Implementation of the memory array module for the UM
 * virtual machine program.
 * Models memory as a dynamic unboxed array of Segment structs,
 * which hold 32-bit words.
 *
 * Authors: Kenneth Xue (kxue01)
 *          William Varteresian (wvarte01)
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "memarray.h"

#define T Memarray_T

static void Memarray_expand(T ma);

/*
 * Returns a newly allocated Memarray_T struct.
 * It initially holds 32 segments,
 * initialized to NULL words and size 0.
 * Input:   nothing
 * Returns: - a Memarray_T struct
 */
T        Memarray_new()
{
        T ma = malloc(sizeof(struct T));
        assert(ma != NULL);
        ma->size = 32;
        ma->cur = 1;
        ma->memory = malloc(sizeof(struct Segment) * ma->size);
        assert(ma->memory != NULL);
        for (uint32_t i= 0; i < ma->size; i++) {
                ma->memory[i].words = NULL;
                ma->memory[i].size = 0;
        }
        ma->unmapped = Seq_seq(NULL);
        assert(ma->unmapped != NULL);
        return ma;
}

/*
 * Frees a memory array. Frees all memory allocated
 * in the array and its segments.
 * Input:   - a Memarray_T struct pointer (a double pointer)
 *          It is a checked runtime error for ma or *ma to be NULL.
 * Returns: nothing
 */
void     Memarray_free(T *ma)
{
        assert(ma != NULL && *ma != NULL);
        T temp = *ma;
        for (uint32_t i = 0; i < temp->size; i++) {
                if (temp->memory[i].words != NULL) {
                        free(temp->memory[i].words);
                }
        }
        free(temp->memory);
        Seq_free(&(temp->unmapped));
        free(*ma);
        ma = NULL;
}

/*
 * Inserts a segment at a given index.
 * If the index is >= the length of the array, the array is expanded.
 * Input:   - a Memarray_T struct
 *          - a uint32_t representing an index
 *          - a pointer to a Segment struct
 *          It is a checked runtime error for ma or s to be NULL.
 * Returns: - a uint32_t representing the index
 *            at which s was successfully inserted.
 */
uint32_t Memarray_map(T ma, uint32_t index, Segment *s)
{
        assert(ma != NULL && s != NULL);        
        if (ma->cur >= ma->size) {
                Memarray_expand(ma);
        }
        ma->memory[index] = *s;
        return index;
}

/*
 * Doubles the size of a memory array,
 * copying all contents of the existing array
 * and initializing all new segments to NULL words and length 0.
 * Input:   - a Memarray_T struct
 *          It is a checked runtime error for ma to be NULL.
 * Returns: nothing
 */
static void Memarray_expand(T ma)
{
        ma->size = ma->size * 2;
        ma->memory = realloc(ma->memory,
                             sizeof(struct Segment)
                             * ma->size);
        assert(ma->memory != NULL);
        for (uint32_t i= ma->cur; i < ma->size; i++) {
                ma->memory[i].words = NULL;
                ma->memory[i].size = 0;
        }
}

/*
 * Frees a segment at a given index of a memory array
 * and adds its index to a sequence of unmapped segment indices.
 * Input:   - a Memarray_T struct
 *          - a uint32_t representing an index
 * Returns: nothing
 */
void     Memarray_unmap(T ma, uint32_t segment)
{
        assert(ma != NULL);
        assert(segment < ma->size);
        free(ma->memory[segment].words);
        ma->memory[segment].words = NULL;
        ma->memory[segment].size = 0;
        if (segment != 0) {
                Seq_addhi(ma->unmapped, (void*)(uintptr_t)segment);
        }
}

/*
 * Returns a pointer to a specified word in a memory array.
 * Input:   - a Memarray_t struct
 *          - a uint32_t representing a segment index
 *          - a uint32_t representing a word index
 *          It is a checked runtime error for ma to be NULL,
 *          for segment to be outside the bounds of ma,
 *          or for word to be outside the bounds of the segment indicated.
 * Returns: - a uint32_t pointer to the word at memory[segment][word]
 */
uint32_t *Memarray_at(T ma, uint32_t segment, uint32_t word)
{
        assert(ma != NULL);
        assert(segment < ma->size);
        assert(word < ma->memory[segment].size);
        return &(ma->memory[segment].words[word]);
}

/*
 * Returns the length of a given segment in a memory array.
 * Input:   - a Memarray_T
 *          - a uint32_t representing a segment index
 *          It is a checked runtime error for ma to be NULL
 *          or for segment to be outside the bounds of ma.
 * Returns: - a uint32_t representing a the length of the segment
 *            at index segment.
 */
uint32_t Memarray_slen(T ma, uint32_t segment)
{
        assert(ma != NULL);
        assert(segment < ma->size);
        return ma->memory[segment].size;
}

#undef T
