#pragma once

#include <stdint.h>
#include "common.h"
#include "condflags.h"

// Extends x to num_bits (up to 16); for negative numbers,
// this entails filling the most significant bits w/ 1s
uint16_t sign_extend(uint16_t x, int num_bits) {
    if ((x >> (num_bits - 1)) & 1) {
        x |= (0xFFFF << num_bits);
    }
    return x;
}

// Updates the flag register after the register
// r has been updated to its current value
void update_flags(uint16_t r) {
    if (registers[r] == 0)
        registers[R_COND] = FL_ZRO;
    else if (registers[r] >> 15) // set most significant bit => negative number
        registers[R_COND] = FL_NEG;
    else
        registers[R_COND] = FL_POS;
}

// TODO: This
uint16_t read_image(const char *path) {
    return 1;
}

uint16_t mem_read(uint16_t address) {
    // TODO: Handle memory-mapped registers
    return memory[address];
}

void mem_write(uint16_t address, uint16_t val) {
    memory[address] = val;
}

