#ifndef CHIP8_H
#define CHIP8_H

#include "config.h"
#include "c8memory.h"
#include "c8registers.h"

struct chip8 {
    struct c8_memory memory;
    struct c8_registers registers;
};

#endif