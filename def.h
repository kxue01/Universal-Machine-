/*
 * def.h
 * Comp 40 HW 6: UM
 * Fall 2019
 *
 * Common definitions to be used by multiple
 * modules of the um virtual machine program.
 *
 * Authors: Kenneth Xue (kxue01)
 *          William Varteresian (wvarte01)
 */

#ifndef _DEF_H_INCLUDED
#define _DEF_H_INCLUDED

/* opcodes to be used by the UM (provided in lab) */
typedef enum Um_opcode {
       CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
       NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

/* a definition to distinguish UM instructions
   from other 32-bit values  */
typedef uint32_t Um_instruction;

#endif
