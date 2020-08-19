/*
 * decipher.h
 * Comp 40 HW 6: UM
 * Fall 2019
 *
 * A module to decipher 32-bit instructions
 * for the um virtual machine.
 *
 * Authors: Kenneth Xue (kxue01)
 *          William Varteresian (wvarte01)
 */

#ifndef DECIPHER_H_INCLUDED
#define DECIPHER_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>
#include "def.h"

/* a struct representing a deciphered UM instruction */
typedef struct Instru {
        char ra, rb, rc; /* register numbers */
        Um_opcode opcode; /* the opcode */
        uint32_t value; /* the value used in load value
                           instructions. 0 if not used */
} Instru;


Instru decipher(uint32_t ins);

#endif
