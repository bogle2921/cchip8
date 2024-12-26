#ifndef C8STACK_H
#define C8STACK_H

#include "config.h"

struct chip8;
struct c8_stack{
    unsigned short stack[STACK_DEPTH];
};

void stack_push(struct chip8* chip8, unsigned short val);
unsigned short stack_pop(struct chip8* chip8);

#endif