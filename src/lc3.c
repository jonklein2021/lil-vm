#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "opcodes.h"
#include "ophandlers.h"

void usage() {
    printf("./lc3 <image-file1> [image-file2] ...\n");
}

int main(int argc, char **argv) {
    // validate command-line arguments
    if (argc < 2) {
        usage();
        exit(2);
    }

    // read images from argument list
    for (int i = 1; i < argc; i++) {
        if (!read_image(argv[i])) {
            fprintf(stderr, "Failed to load image file: %s\n", argv[i]);
            exit(1);
        }
    }

    // initialize condition flag
    registers[R_COND] = FL_ZRO;

    // initialize PC
    registers[R_PC] = PC_START;

    // array of operation handler functions
    void (*handlers[OP_COUNT])(uint16_t) = {
        handle_add,
        handle_and,
        handle_not,
        handle_br,
        handle_jmp,
        handle_jsr,
        handle_ld,
        handle_ldi,
        handle_ldr,
        handle_lea,
        handle_st,
        handle_sti,
        handle_str,
        handle_trp,
        handle_res,
        handle_rti
    };

    while (1) {
        // fetch instruction and get correspondent operation
        uint16_t instr = mem_read(registers[R_PC]++);
        uint16_t op = instr >> 12;

        // perform correspondent operation
        handlers[op](instr);
    }
    
}

