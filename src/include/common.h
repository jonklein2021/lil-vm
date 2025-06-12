#pragma once

#include <stdint.h>
#include "constants.h"
#include "registers.h"

uint16_t registers[R_COUNT];
uint16_t memory[MEMORY_MAX];

int running = 1;

