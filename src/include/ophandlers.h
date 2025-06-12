#pragma once

#include <stdint.h>
#include "common.h"
#include "condflags.h"
#include "trapcodes.h"
#include "traproutines.h"
#include "utils.h"

void handle_add(uint16_t instr) {
    uint16_t r_dest = (instr >> 9) & 0x7;
    uint16_t r_input1 = (instr >> 6) & 0x7;
    uint16_t imm_mode = (instr >> 5) & 0x1;

    if (imm_mode) {
        uint16_t imm5 = sign_extend(instr & 0x1F, 5); // 5-bit immediate value
        registers[r_dest] = registers[r_input1] + imm5;
    } else {
        uint16_t r_input2 = instr & 0x7;
        registers[r_dest] = registers[r_input1] + registers[r_input2];
    }

    update_flags(r_dest);
}

void handle_and(uint16_t instr) {
    uint16_t r_dest = (instr >> 9) & 0x7;
    uint16_t r_input1 = (instr >> 6) & 0x7;
    uint16_t imm_mode = (instr >> 5) & 0x1;

    if (imm_mode) {
        uint16_t imm5 = sign_extend(instr & 0x1F, 5); // 5-bit immediate value
        registers[r_dest] = registers[r_input1] & imm5;
    } else {
        uint16_t r_input2 = instr & 0x7;
        registers[r_dest] = registers[r_input1] & registers[r_input2];
    }

    update_flags(r_dest);

}

void handle_not(uint16_t instr) {
    uint16_t r_dest = (instr >> 9) & 0x7;
    uint16_t r_input = (instr >> 6) & 0x7;

    registers[r_dest] = ~registers[r_input];
    update_flags(r_dest);
}

void handle_br(uint16_t instr) {
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
    uint16_t cond_flag = (instr >> 9) & 0x7;
    if (cond_flag & registers[R_COND]) {
        registers[R_PC] += pc_offset;
    }
}

// also handles RET instruction
void handle_jmp(uint16_t instr) {
    uint16_t r_label = (instr >> 6) & 0x7;
    registers[R_PC] = registers[r_label];
}

void handle_jsr(uint16_t instr) {
    uint16_t long_flag = (instr >> 11) & 1;
    registers[R_R7] = registers[R_PC];
    if (long_flag) {
        // JSR
        uint16_t long_pc_offset = sign_extend(instr & 0x7FF, 11);
        registers[R_PC] += long_pc_offset; 
    } else {
        // JSRR
        uint16_t r1 = (instr >> 6) & 0x7;
        registers[R_PC] = registers[r1]; 
    }
}

void handle_ld(uint16_t instr) {
    uint16_t r_dest = (instr >> 9) & 0x7;
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
    registers[r_dest] = mem_read(registers[R_PC] + pc_offset);
    update_flags(r_dest);
}

void handle_ldi(uint16_t instr) {
    uint16_t r_dest = (instr >> 9) & 0x7;
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);

    // 2 layers of indirection allows us to read from more addresses
    // 1 layer (i.e. what LD does) only allows for 2^9 addresses
    registers[r_dest] = mem_read(mem_read(registers[R_PC] + pc_offset));
    update_flags(r_dest);
}

// loads the content located at the address stored in a
// register plus some offset into a destination register
void handle_ldr(uint16_t instr) {
    uint16_t r_dest = (instr >> 9) & 0x7;
    uint16_t r_src = (instr >> 6) & 0x7;
    uint16_t offset = sign_extend(instr & 0x3F, 6);
    registers[r_dest] = mem_read(registers[r_src] + offset);
    update_flags(r_dest);
}

// loads the address stored at a register plus some offset
// into a destination register
void handle_lea(uint16_t instr) {
    uint16_t r_dest = (instr >> 9) & 0x7;
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
    registers[r_dest] = registers[R_PC] + pc_offset;
    update_flags(r_dest);
}

// stores the value of a register at some offset in
// memory, where the PC indicates the beginning of memory
void handle_st(uint16_t instr) {
    uint16_t r_val = (instr >> 9) & 0x7;
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
    mem_write(registers[R_PC] + pc_offset, registers[r_val]);
}

// same as ST but with a layer of indirection
void handle_sti(uint16_t instr) {
    uint16_t r_val = (instr >> 9) & 0x7;
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
    mem_write(mem_read(registers[R_PC] + pc_offset), registers[r_val]);
}

// stores the value in a register at the address held by another
// register plus some offset
void handle_str(uint16_t instr) {
    uint16_t r_val = (instr >> 9) & 0x7;
    uint16_t r_addr = (instr >> 6) & 0x7;
    uint16_t offset = sign_extend(instr & 0x3F, 6);
    mem_write(registers[r_addr] + offset, registers[r_val]);
}

void handle_trp(uint16_t instr) {
    void (*routines[NUM_TRAP])() = {
        trap_getc,
        trap_out,
        trap_puts,
        trap_in,
        trap_putsp,
        trap_halt,
    };

    // save PC position in R7
    // N.B: do I need to update the cond flags?
    registers[R_R7] = registers[R_PC];

    // call correspondent trap routine
    routines[instr & 0xFF]();
}

void handle_res(uint16_t instr) {
    abort();
}

void handle_rti(uint16_t instr) {
    abort();
}


