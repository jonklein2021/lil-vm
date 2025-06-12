#pragma once

#include "common.h"
#include "utils.h"

// read a singular ASCII character from stdin
void trap_getc() {
    registers[R_R0] = (uint16_t) getchar();
    update_flags(R_R0);
}

// output a single ASCII character to stdout and flush it
void trap_out() {
    putc((char) registers[R_R0], stdout);
    fflush(stdout);
}

// same as puts function in C; also flushes
// stdout after printing
void trap_puts() {
    // get first character from memory,
    // which is at the offset stored in R0
    uint16_t* c = memory + registers[R_R0];
    
    // print characters until null byte is reached
    while (*c) {
        putc((char)*c, stdout);
        ++c;
    }

    fflush(stdout);
}

// prompt user to input a character, read from stdin into
// R0, output the character to stdout and flush it
void trap_in() {
    // prompt user for input and read from stdin
    printf("Enter a character: ");
    char c = getchar();

    // print char to stdout and flush it
    putc(c, stdout);
    fflush(stdout);

    // store char in R0 and update cond flags
    registers[R_R0] = (uint16_t) c;
    update_flags(R_R0);
}

// print string to stdout
// N.B: our system is 16-bit so there are two chars per word;
// we must also remember to swap back to big endian
void trap_putsp() {
    uint16_t* c = memory + registers[R_R0];
    
    // print until null byte is reached
    while (*c) {
        char char1 = (*c) & 0xFF; // first byte of 16-bit word
        putc(char1, stdout);

        char char2 = (*c) >> 8; // second byte of the same word
        if (char2) putc(char2, stdout);
        c++;
    }

    fflush(stdout);
}

void trap_halt() {
    puts("HALT");
    fflush(stdout);
    exit(0); // indicate graceful exit
}

