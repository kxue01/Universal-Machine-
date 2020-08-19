/*
 * um.c
 * Comp 40 HW 6: UM
 * Fall 2019
 *
 * The main driver for the um program,
 * a virtual machine with a 32-bit architecture.
 *
 * Authors: Kenneth Xue (kxue01)
 *          William Varteresian (wvarte01)
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <seq.h>
#include "assert.h"
#include "except.h"
#include "memarray.h"
#include "memory.h"
#include "decipher.h"
#include "operate.h"
#include "def.h"

void init_program(FILE *fp, Segment *program);
uint32_t execute_ins(Instru cur, Memarray_T ma, uint32_t counter);

uint32_t registers [8] = {0, 0, 0, 0, 0, 0, 0, 0};

int main (int argc, char* argv[])
{
        Memarray_T memory;
        Segment program;
        FILE *fp;
        Instru cur = {0, 0, 0, 0, 0};
        uint32_t counter = 0;
        if (argc != 2) {
                fprintf(stderr, "usage: %s [um program file]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
        fp = fopen(argv[1], "r");
        assert(fp != NULL);
        init_program(fp, &program);
        fclose(fp);
        memory = Memarray_new();
        assert(memory != NULL);
        Memarray_map(memory, 0, &program);
        while (cur.opcode != HALT) {
                uint32_t *i = Memarray_at(memory, 0, counter);
                cur = decipher(*i);
                counter = execute_ins(cur, memory, counter);
        }
        Memarray_free(&memory);
        return EXIT_SUCCESS;
}

/* Input: file pointer fp, Struct segment representing program
 * Output: nothing
 * Does: Reads from the file and in order loads the instructions into
 *       a seq_t structure, then once all the instructions are read,
 *       loads instructions in order to an malloc'ed array and
 *       frees the Seq_T. This gets us our initial program at segment 0 
 */
void init_program(FILE *fp, Segment *program)
{
        Seq_T s = Seq_seq(NULL);
        assert(s != NULL);
        int c = 0;
        while (c != EOF) {
                Um_instruction instruction = 0;
                for (int i = 3; i >= 0; i--) {
                        c = fgetc(fp);
                        if (c == EOF) {
                                break;
                        }
                instruction = instruction | (c << (8 * i));
                }
                Seq_addhi(s, (void*)(uintptr_t)instruction);
        }
        program->size = Seq_length(s);
        program->words = malloc(sizeof(Um_instruction) * program->size);
        assert(program->words != NULL);
        for (unsigned i = 0; i < program->size; i++) {
                program->words[i] = (Um_instruction)(uintptr_t)Seq_remlo(s);
        }
        Seq_free(&s);
}

/* Input: struct instru for current instruction, memarray struct, 
 *        counter for current instruction
 * Output: updated counter
 * Does: looks at the current instruction's opcode, and calls different
 *       modules/functions to execute the instruction. Function also
 *       updates the instruction counter
 */
uint32_t execute_ins(Instru cur, Memarray_T ma, uint32_t counter)
{
        counter++;
        if (cur.opcode == CMOV || (cur.opcode >= ADD && cur.opcode <= NAND)) {
                operate(cur. opcode, &(registers[(int)cur.ra]),
                        &(registers[(int)cur.rb]), &(registers[(int)cur.rc]));
        } else if (cur.opcode == SLOAD) {
                seg_load(ma, &(registers[(int)cur.ra]), &(registers[(int)cur.rb]),
                        &(registers[(int)cur.rc]));
        } else if (cur.opcode == SSTORE) {
                seg_store(ma, &(registers[(int)cur.ra]), &(registers[(int)cur.rb]),
                        &(registers[(int)cur.rc]));
        } else if (cur.opcode == ACTIVATE) {
                map_segment(ma, &(registers[(int)cur.rb]), &(registers[(int)cur.rc]));
        } else if (cur.opcode == INACTIVATE) {
                unmap_segment(ma, &(registers[(int)cur.rc]));
        } else if (cur.opcode == OUT) {
                if (registers[(int)cur.rc] < 256) {
                        fputc(registers[(int)cur.rc], stdout);
                } else {
                        exit(EXIT_FAILURE);
                }
        } else if (cur.opcode == IN) {
                int c = fgetc(stdin);
                registers[(int)cur.rc] = c;
        } else if (cur.opcode == LOADP) {
                if (registers[(int)cur.rb] != 0) {
                        load_program(ma, registers[(int)cur.rb]);
                }
                counter = registers[(int)cur.rc];
        } else if (cur.opcode == LV) {
                registers[(int)cur.ra] = cur.value;
        }
        return counter;
}
