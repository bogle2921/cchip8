#ifndef CHIP8_H
#define CHIP8_H

#include "config.h"
#include "c8memory.h"
#include "c8registers.h"
#include "c8stack.h"
#include "c8keyboard.h"

struct chip8 {
    struct c8_memory memory;
    struct c8_registers registers;
    struct c8_stack stack;
    struct c8_keyboard keyboard;
};

void chip8_init(struct chip8* chip8);
#endif