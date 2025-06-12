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
   
    puts("[lil-vm] Reading images...");

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

    // disable input buffering
    signal(SIGINT, handle_interrupt);
    disable_input_buffering();

    puts("[lil-vm] Running instructions...");

    // array of operation handler functions
    void (*handlers[OP_COUNT])(uint16_t) = {
        handle_br, // branch
        handle_add, // add
        handle_ld, // load
        handle_st, // store
        handle_jsr, // jump register
        handle_and, // bitwise and
        handle_ldr, // load (memory to register)
        handle_str, // store (register to memory)
        handle_rti, // unused
        handle_not, // bitwise not
        handle_ldi, // load indirect
        handle_sti, // store indirect
        handle_jmp, // jump
        handle_res, // reserved (unused)
        handle_lea, // load effective address
        handle_trp, // execute trap
    };

    while (running) { // running is a global var so trap routines can access it
        // fetch instruction and get correspondent operation
        uint16_t instr = mem_read(registers[R_PC]++);
        uint16_t op = (instr >> 12) & 0xF; // ops are 4-bits each
    
        // ensure operation is in array bounds
        if (op >= OP_COUNT) {
            fprintf(stderr, "Unknown opcode 0x%X at PC=0x%X\n", instr, registers[R_PC] - 1);
            abort();
        }

        // perform correspondent operation
        handlers[op](instr);
    }

    restore_input_buffering();    
}

