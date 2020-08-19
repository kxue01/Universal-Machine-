/*
 * umlab.c
 * 
 * The functions defined in this lab should be linked against umlabwrite.c
 * to produce a unit test writing program. Any additional functions and unit
 * tests written for the lab go here.
 */

#include <stdint.h>
#include <stdio.h>
#include <bitpack.h>
#include <assert.h>
#include <seq.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc);
Um_instruction loadval(unsigned ra, unsigned val);

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        uint64_t inst = Bitpack_newu(0, 4, 28, op);
        inst = Bitpack_newu(inst, 3, 0, rc);
        inst = Bitpack_newu(inst, 3, 3, rb);
        inst = Bitpack_newu(inst, 3, 6, ra);
        return (Um_instruction)inst;
}

Um_instruction loadval(unsigned ra, unsigned val)
{
        uint64_t inst = Bitpack_newu(0, 25, 0, val);
        inst = Bitpack_newu(inst, 3, 25, ra);
        inst = Bitpack_newu(inst, 4, 28, LV);
        return (Um_instruction)inst;
}

/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

Um_instruction input(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

Um_instruction nand(Um_register a, Um_register b, Um_register c)
{
        return three_register(NAND, a, b, c);
}

Um_instruction cmov(Um_register a, Um_register b, Um_register c)
{
        return three_register(CMOV, a, b, c);
}

Um_instruction loadp(Um_register b, Um_register c)
{
        return three_register(LOADP, 0, b, c);
}

/* Functions for working with streams */

static inline void emit(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

/*
 * Writes a sequence of UM instructions to a file.
 * Instructions are written in big endian order.
 * Instructions are written in the order
 * they are stored in the sequence.
 * Input:   - a FILE pointer to the output file
 *          - a Seq_T containing a sequence of Um_instructions
 * Returns: nothing
 */
void Um_write_sequence(FILE *output, Seq_T stream)
{
        while (Seq_length(stream) > 0) {
                uintptr_t i = (uintptr_t)Seq_remlo(stream);
                Um_instruction umi = i;
                for (int i = 8; i <= 32; i += 8) {
                        fputc(Bitpack_getu(umi, 8, 32 - i), output);
                }
        }
}

/* Unit tests for the UM */

void emit_halt_test(Seq_T stream)
{
        emit(stream, halt());
}

void emit_verbose_halt_test(Seq_T stream)
{
        emit(stream, halt());
        emit(stream, loadval(r1, 'B'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'a'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'd'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '!'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));
}

void emit_add_test(Seq_T stream)
{
        emit(stream, loadval(r2, 50));
        emit(stream, loadval(r3, 5));
        emit(stream, add(r1, r2, r3));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_print_six(Seq_T stream)
{
        emit(stream, loadval(r1, 48));
        emit(stream, loadval(r2, 6));
        emit(stream, add(r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}

void emit_out_test(Seq_T stream)
{
        emit(stream, loadval(r6, 1));
        emit(stream, loadval(r5, 7));
        emit(stream, input(r2));
        emit(stream, output(r2));
        emit(stream, nand(r4, r2, r2));
        emit(stream, cmov(r5, r6, r4));
        emit(stream, loadp(r0, r5));
        emit(stream, halt());
}

void emit_nand_test(Seq_T stream)
{
        emit(stream, loadval(r1, 111));
        emit(stream, loadval(r2, 111));
        emit(stream, loadval(r3, 65));
        emit(stream, nand(r0, r1, r2));
        emit(stream, add(r0, r0, r3));
        emit(stream, output(r0));
        emit(stream, halt());
}

void emit_mult_test(Seq_T stream)
{
        emit(stream, loadval(r0, ));
        emit();
}
