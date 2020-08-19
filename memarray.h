/*
 * memarray.h
 * Comp 40 HW6: UM
 * Fall 2019
 *
 * Interface of the memory array module for the UM
 * virtual machine program.
 * Models memory as a dynamic unboxed array of Segment structs,
 * which hold 32-bit words.
 *
 * Authors: Kenneth Xue (kxue01)
 *          William Varteresian (wvarte01)
 */

#ifndef _MEMARRAY_H_INCLUDED
#define _MEMARRAY_H_INCLUDED

#include <stdint.h>
#include <seq.h>

#define T Memarray_T

/* Models a segment of memory */
typedef struct Segment {
        uint32_t *words; /* An array of 32-bit words */
        uint32_t size;   /* The length of the words array */
} Segment;

/* Models the memory of a 32-bit machine */
typedef struct T {
        Segment *memory;    /* A dynamic array of segments */
        uint32_t size, cur; /* The size of the array
                               and the index of
                               the next unmapped segment */
        Seq_T unmapped;     /* A sequence of indices
                               that have been unmapped */
} *T;

/*
 * Returns a newly allocated Memarray_T struct.
 * It initially holds 32 segments,
 * initialized to NULL words and size 0.
 * Input:   nothing
 * Returns: - a Memarray_T struct
 */
T        Memarray_new();

/*
 * Frees a memory array. Frees all memory allocated
 * in the array and its segments.
 * Input:   - a Memarray_T struct pointer (a double pointer)
 *          It is a checked runtime error for ma or *ma to be NULL.
 * Returns: nothing
 */
void     Memarray_free(T*);

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
uint32_t Memarray_map(T, uint32_t, Segment*);

/*
 * Frees a segment at a given index of a memory array
 * and adds its index to a sequence of unmapped segment indices.
 * Input:   - a Memarray_T struct
 *          - a uint32_t representing an index
 * Returns: nothing
 */
void     Memarray_unmap(T, uint32_t);

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
uint32_t *Memarray_at(T, uint32_t, uint32_t);

/*
 * Returns the length of a given segment in a memory array.
 * Input:   - a Memarray_T
 *          - a uint32_t representing a segment index
 *          It is a checked runtime error for ma to be NULL
 *          or for segment to be outside the bounds of ma.
 * Returns: - a uint32_t representing a the length of the segment
 *            at index segment.
 */
uint32_t Memarray_slen(T, uint32_t);

#undef T
#endif
