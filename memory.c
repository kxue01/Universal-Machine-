/*
 * memory.c
 * Comp 40 HW6: UM
 * Fall 2019
 *
 * Implementation of the memory module for the UM virtual machine
 * program. Includes all functions that directly access memory.
 *
 * Authors: Kenneth Xue (kxue01)
 *          William Varteresian (wvarte01)
 */

#include <stdlib.h>
#include <stdio.h>
#include "memory.h"
#include <assert.h>

/*
 * Loads the value at segment memory[Register B][Register C]
 * into Register A.
 * Input:   - a Memarray_T struct (a pointer by default)
 *          - three uint32_t pointers representing registers
 *            in a 32-bit machine
 *          It is a checked runtime error for any argument to be NULL.
 * Returns: nothing
 */
void seg_load(Memarray_T ma, uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        assert(ma != NULL && ra != NULL && rb != NULL && rc != NULL);
        uint32_t *value = Memarray_at(ma, *rb, *rc);
        *ra = *value;
}

/*
 * Loads the value in Register C into memory[Register A][Register B].
 * Input:   - a Memarray_T struct (a pointer by default)
 *          - three uint32_t pointers representing registers
 *            in a 32-bit machine
 *          It is a checked runtime error for any argument to be NULL.
 * Returns: nothing
 */
void seg_store(Memarray_T ma, uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        assert(ma != NULL && ra != NULL && rb != NULL && rc != NULL);
        uint32_t *value = Memarray_at(ma, *ra, *rb);
        *value = *rc;
}

/*
 * Allocates a segment of length equal to the value
 * in register C in an array representing 32-bit memory
 * and stores the index (address) of that segment
 * in register B.
 * All words in the new segment are initialized to 0.
 * Input:   - a memarray struct (a pointer by default)
 *          - two pointers to 32-bit unsigned ints
 *            representing registers
 *          It is a checked runtime error for any argument to be NULL.
 * Returns: nothing
 */
void map_segment(Memarray_T ma, uint32_t *rb, uint32_t *rc)
{
        assert(ma != NULL && rb != NULL && rc != NULL);
        uint32_t index;
        Segment s;
        s.size = *rc;
        s.words = malloc(sizeof(uint32_t) * (*rc));
        assert(s.words != NULL);
        for (uint32_t i = 0; i < s.size; i++) {
                s.words[i] = 0;
        }
        if (Seq_length(ma->unmapped) > 0) {
                index = (uint32_t)(uintptr_t)Seq_remlo(ma->unmapped);
        } else {
                index = ma->cur;
                ma->cur++;
        }
        *rb = Memarray_map(ma, index, &s);
}

/*
 * Frees a segment at the index (adress) stored in register C
 * in an array representing 32-bit memory
 * and releases its index (address) to be used by future calls
 * to map_segment.
 * Input:   - a memarray struct (a pointer by default)
 *          - a pointer to a 32-bit unsigned int representing
 *            a register
 *          It is a checked runtime error for any argument to be NULL.
 *          It is a checked runtime error for *rc to be 0
 *          or the index of an unmapped segment.
 * Returns: nothing
 */
void unmap_segment(Memarray_T ma, uint32_t *rc)
{
        assert(ma != NULL && rc != NULL);
        Memarray_unmap(ma, *rc);
}

/*
 * Loads a program at memory[Register B] into memory[0].
 * The current program at memory[0] is freed,
 * and the segment at memory[Register B] is copied, no moved.
 * Input:   - a Memarray_T struct (a pointer by default)
 *          - a uint32_t representing the contents
 *            of a register in a 32-bit machine
 *          It is a checked runtime error for ma to be NULL.
 * Returns: nothing
 */
void load_program(Memarray_T ma, uint32_t rb)
{
        assert(ma != NULL);
        Memarray_unmap(ma, 0);
        Segment s;
        s.size = Memarray_slen(ma, rb);
        s.words = malloc(sizeof(uint32_t) * s.size);
        for (uint32_t i = 0; i < s.size; i++) {
                uint32_t *temp = Memarray_at(ma, rb, i);
                s.words[i] = *temp;
        }
        Memarray_map(ma, 0, &s);
}
