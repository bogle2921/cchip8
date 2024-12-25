#ifndef C8REGISTERS_H
#define C8REGISTERS_H

#include "config.h"

struct c8_registers{
    unsigned char V[TOTAL_REGISTERS];
    unsigned short I;
    unsigned char dt; // delay timer
    unsigned char st; // sound timer
    unsigned short PC;
    unsigned char SP;
};
#endif