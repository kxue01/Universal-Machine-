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

#include "bitpack.h"
#include <assert.h>
#include "decipher.h"


/* Input: an uint32_t instruction
 * Output: a struct Instru containing ra,rb,rc,opcode, and value
 * Does: uses bitpack module to extract information from each instruction
 */
Instru decipher(Um_instruction ins)
{
    Instru i;
    i.opcode = (Um_opcode)Bitpack_getu(ins, 4, 28);
    assert(i.opcode <= LV);
    if (i.opcode == LV) {
        i.ra = Bitpack_getu(ins, 3, 25);
        i.rb = 0;
        i.rc = 0;
        i.value = Bitpack_getu(ins, 25, 0);
    } else {
        i.ra = Bitpack_getu(ins, 3, 6);
        i.rb = Bitpack_getu(ins, 3, 3);
        i.rc = Bitpack_getu(ins, 3, 0);
        i.value = 0;
    }
    return i;
}
