#pragma once

enum {
    OP_ADD, // add
    OP_AND, // bitwise and
    OP_NOT, // bitwise not
    OP_BR, // branch
    OP_JMP, // jump
    OP_JSR, // jump register
    OP_LD, // load
    OP_LDI, // load indirect
    OP_LDR, // load (memory to register)
    OP_LEA, // load effective address
    OP_ST, // store
    OP_STI, // store indirect
    OP_STR, // store (register to memory)
    OP_TRP, // execute trap
    OP_RES, // reserved (unused)
    OP_RTI, // unused
    OP_COUNT
};

