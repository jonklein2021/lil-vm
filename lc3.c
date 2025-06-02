#include <stdio.h>
#include "constants.h"
#include "registers.h"
#include "condflags.h"
#include "opcodes.h"

uint16_t memory[MEMORY_MAX];
uint16_t registers[R_COUNT];

void usage() {
    printf("./lc3 [image-file1] ...\n");
}

int main(int argc, char **argv) {
    // validate command-line arguments
    if (argc < 2) {
        usage();
        exit(1);
    }

    // initialize condition flag
    registers[R_COND] = FL_ZRO;

    // initialize PC
    registers[R_PC] = PC_START;

    while (1) {
        // fetch instruction and get correspondent operation
        uint16_t instr = mem_read(reg[R_PC++]);
        uint16_t op = instr >> 12;

        // perform correspondent operation
        switch (op) {
            case OP_ADD:
                break;

            case OP_AND:
                break;

            case OP_NOT:
                break;

            case OP_BR:
                break;

            case OP_JMP:
                break;

            case OP_JSR:
                break;

            case OP_LD:
                break;

            case OP_LDI:
                break;

            case OP_LDR:
                break;

            case OP_LEA:
                break;

            case OP_ST:
                break;

            case OP_STI:
                break;

            case OP_STR:
                break;

            case OP_TRAP:
                break;

            case OP_RES:
            case OP_RTI:
                break;

            default:
                // bad opcode
                break;               

        }

    }
}

