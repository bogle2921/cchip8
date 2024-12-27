#ifndef C8MEMORY_H
#define C8MEMORY_H

#include "config.h"
struct c8_memory {
    unsigned char memory[MEMORY_SIZE];
};

void set_memory(struct c8_memory* memory, int index, unsigned char val);
unsigned char get_memory(struct c8_memory* memory, int index);
unsigned short get_memory_short(struct c8_memory* memory, int index);

#endif
