#pragma once

#include <stdint.h>
#include "common.h"
#include "condflags.h"
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

void handle_and(uint16_t instr);
void handle_not(uint16_t instr);
void handle_br(uint16_t instr);
void handle_jmp(uint16_t instr);
void handle_jsr(uint16_t instr);
void handle_ld(uint16_t instr);

void handle_ldi(uint16_t instr) {
    uint16_t r_dest = (instr >> 9) & 0x7;
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);

    // 2 layers of indirection allows us to read from more addresses
    // 1 layer of (i.e. one mem_read) would only allow for 2^9 addresses
    registers[r_dest] = mem_read(mem_read(registers[R_PC] + pc_offset));
    update_flags(r_dest);
}

void handle_ldr(uint16_t instr);
void handle_lea(uint16_t instr);
void handle_st(uint16_t instr);
void handle_sti(uint16_t instr);
void handle_str(uint16_t instr);
void handle_trp(uint16_t instr);
void handle_res(uint16_t instr);
void handle_rti(uint16_t instr);


