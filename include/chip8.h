#ifndef CHIP8_H
#define CHIP8_H

#include <stddef.h>
#include "config.h"
#include "c8memory.h"
#include "c8registers.h"
#include "c8stack.h"
#include "c8keyboard.h"
#include "c8screen.h"

struct chip8 {
    struct c8_memory memory;
    struct c8_registers registers;
    struct c8_stack stack;
    struct c8_keyboard keyboard;
    struct c8_screen screen;
};

void chip8_init(struct chip8* chip8);
void chip8_load(struct chip8* chip8, const char* buf, size_t size);
void chip8_exec(struct chip8* chip8, unsigned short opcode);
#endif