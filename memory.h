/*
 * memory.h
 * Comp 40 HW6: UM
 * Fall 2019
 *
 * Interface of the memory module for the UM virtual machine
 * program. Includes all functions that directly access memory.
 *
 * Authors: Kenneth Xue (kxue01)
 *          William Varteresian (wvarte01)
 */


#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>
#include "memarray.h"

/*
 * Loads the value at segment memory[Register B][Register C]
 * into Register A.
 * Input:   - a Memarray_T struct (a pointer by default)
 *          - three uint32_t pointers representing registers
 *            in a 32-bit machine
 *          It is a checked runtime error for any argument to be NULL.
 * Returns: nothing
 */
void seg_load(Memarray_T ma, uint32_t *ra, uint32_t *rb, uint32_t *rc);

/*
 * Loads the value in Register C into memory[Register A][Register B].
 * Input:   - a Memarray_T struct (a pointer by default)
 *          - three uint32_t pointers representing registers
 *            in a 32-bit machine
 *          It is a checked runtime error for any argument to be NULL.
 * Returns: nothing
 */
void seg_store(Memarray_T ma, uint32_t *ra, uint32_t *rb, uint32_t *rc);

/*
 * Allocates a segment of length equal to the value
 * in register C in an array representing 32-bit memory
 * and stores the index (address) of that segment
 * in register B.
 * Input:   - a memarray struct (a pointer by default)
 *          - two pointers to 32-bit unsigned ints
 *            representing registers
 *          It is a checked runtime error for any argument to be NULL.
 * Returns: nothing
 */
void map_segment(Memarray_T ma, uint32_t *rb, uint32_t *rc);

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
void unmap_segment(Memarray_T ma, uint32_t *rc);

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
void load_program(Memarray_T ma, uint32_t rb);

#endif
