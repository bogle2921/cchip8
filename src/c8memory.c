#include "c8memory.h"
#include <assert.h>

static void memory_in_bounds(int index){
    assert(index >= 0 && index < MEMORY_SIZE);
}
void set_memory(struct c8_memory* memory, int index, unsigned char val){
    memory_in_bounds(index);
    memory->memory[index] = val;
}

unsigned char get_memory(struct c8_memory* memory, int index){
    memory_in_bounds(index);
    return memory->memory[index];
}

unsigned short get_memory_short(struct c8_memory* memory, int index){
    unsigned char byte1 = get_memory(memory, index);
    unsigned char byte2 = get_memory(memory, index+1);
    return byte1 << 8 | byte2;
}