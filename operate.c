/*
 * operate.c
 * Comp 40 HW6: UM
 * Fall 2019
 *
 * Implementation of the operate module for the UM virtual machine
 * program. Includes the conditional move, addition, multiplication,
 * division, and NAND operations of the UM.
 *
 * Authors: Kenneth Xue (kxue01)
 *          William Varteresian (wvarte01)
 */

#include <stdio.h>
#include <assert.h>
#include "operate.h"

/*
 * Performs operations on registers depending on the 
 * Conditional Move: if rc != 0, move rb into ra
 * Addition: add rb and rc and put into ra
 * multiplication: multiply rb and rc and put result int ra
 * division: divide rb and rc and put result into ra
 * NAND: NAND bitwise rb and rc and put result into ra
 *
 * Input:   - uint32_t representing the opcode
 *          - 3 unint32_t representing the registers a,b,c
 *          It is a checked runtime error for any argument to be NULL.
 * Returns: nothing
 */
void operate(uint32_t opcode, uint32_t* ra, uint32_t* rb, uint32_t* rc)
{
        assert(ra != NULL && rb != NULL && rc != NULL);
        if (opcode == CMOV && *rc != 0) { /*conditional move*/
            *ra = *rb;
        }
        else if (opcode == ADD) { /*addition*/
            *ra = (*rb) + (*rc);
        }
        else if (opcode == MUL) { /*multiplication*/
            *ra = (*rb) * (*rc);
        }
        else if (opcode == DIV) { /*division*/
            *ra = (*rb) / (*rc);
        }
        else if (opcode == NAND) { /*Bitwise NAND*/
            *ra = ~((*rb) & (*rc));
        }
}
