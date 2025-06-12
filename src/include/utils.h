#pragma once

#include <stdint.h>
#include "common.h"
#include "condflags.h"
#include "keyboard.h"

// extends x to num_bits (up to 16); for negative numbers,
// this entails filling the most significant bits w/ 1s
uint16_t sign_extend(uint16_t x, int num_bits) {
    if ((x >> (num_bits - 1)) & 1) {
        x |= (0xFFFF << num_bits);
    }
    return x;
}

// converts a word to the other endian
uint16_t swap16(uint16_t x) {
    return (x << 8) | (x >> 8);
}

// updates the flag register after the register
// r has been updated to its current value
void update_flags(uint16_t r) {
    if (registers[r] == 0)
        registers[R_COND] = FL_ZRO;
    else if (registers[r] >> 15) // set most significant bit => negative number
        registers[R_COND] = FL_NEG;
    else
        registers[R_COND] = FL_POS;
}

// loads an LC3 image file into memory 
int read_image(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) return 0;
    
    // location in memory to place the image
    uint16_t origin;
    fread(&origin, sizeof(origin), 1, file);
    origin = swap16(origin); // comment this out on big endian machines

    uint16_t max_read = MEMORY_MAX - origin;
    uint16_t* p = memory + origin;
    size_t num_bytes = fread(p, sizeof(uint16_t), max_read, file);

    // convert to big endian; comment this loop out
    // on exotic machines running big endian already
    while (num_bytes-- > 0) {
        *p = swap16(*p);
        ++p;
    }

    fclose(file);
    return 1;
}

uint16_t mem_read(uint16_t address) {
    if (address == MR_KBSR) {
        if (check_key()) {
            memory[MR_KBSR] = (1 << 15);
            memory[MR_KBDR] = getchar();
        } else {
            memory[MR_KBSR] = 0;
        }
    }
    return memory[address];
}

void mem_write(uint16_t address, uint16_t val) {
    memory[address] = val;
}

