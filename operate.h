/*
 * operate.h
 * Comp 40 HW6: UM
 * Fall 2019
 *
 * Interface of the operate module for the UM virtual machine
 * program. Includes the conditional move, addition, multiplication,
 * division, and NAND operations of the UM.
 *
 * Authors: Kenneth Xue (kxue01)
 *          William Varteresian (wvarte01)
 */


#ifndef OPERATE_H_INCLUDED
#define OPERATE_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>
#include "def.h"

void operate(Um_opcode opcode, uint32_t *ra, uint32_t *rb, uint32_t *rc);

#endif
