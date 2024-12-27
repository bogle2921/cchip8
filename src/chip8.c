#include "chip8.h"
#include <memory.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"

const char default_char_set[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xf0, 0x10, 0xf0, 0x80, 0xf0,
    0xf0, 0x10, 0xf0, 0x10, 0xf0,
    0x90, 0x90, 0xf0, 0x10, 0x10,
    0xf0, 0x80, 0xf0, 0x10, 0xf0,
    0xf0, 0x80, 0xf0, 0x90, 0xf0,
    0xf0, 0x10, 0x20, 0x40, 0x40,
    0xf0, 0x90, 0xf0, 0x90, 0xf0,
    0xf0, 0x90, 0xf0, 0x10, 0xf0,
    0xf0, 0x90, 0xf0, 0x90, 0x90,
    0xe0, 0x90, 0xe0, 0x90, 0xe0,
    0xf0, 0x80, 0x80, 0x80, 0xf0,
    0xe0, 0x90, 0x90, 0x90, 0xe0,
    0xf0, 0x80, 0xf0, 0x80, 0xf0, 
    0xf0, 0x80, 0xf0, 0x80, 0x80,
};

static void chip8_exec_eight(struct chip8* chip8, unsigned short opcode){
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char final_bits = opcode & 0x000f;
    unsigned short tmp = 0;

    switch(final_bits){
        case 0x00: // LD Vx Vy - 8xy0 Vx = Vy
            chip8->registers.V[x] = chip8->registers.V[y];
            break;

        case 0x01: // OR Vx Vy - 8xy1 Vx |= Vy
            chip8->registers.V[x] |= chip8->registers.V[y];
            break;

        case 0x02: // AND Vx Vy - 8xy2 Vx &= Vy
            chip8->registers.V[x] &= chip8->registers.V[y];
            break;
        
        case 0x03: // XOR Vx Vy - 8xy3 Vx ^= Vy
            chip8->registers.V[x] ^= chip8->registers.V[y];
            break;
        
        case 0x04: // ADD Vx Vy - 8xy4 Vx += Vy, VF = carry
            tmp = chip8->registers.V[x] + chip8->registers.V[y];
            chip8->registers.V[0x0f] = false;
            if(tmp > 0xff){
                chip8->registers.V[0x0f] = true;
            }
            chip8->registers.V[x] = tmp;
            break;
        
        case 0x05: // SUB Vx Vy - 8xy5 Vx -= Vy, VF = NOT borrow
            chip8->registers.V[0x0f] = false;
            if (chip8->registers.V[x] > chip8->registers.V[y]){
                chip8->registers.V[0x0f] = true;
            }
            chip8->registers.V[x] -= chip8->registers.V[y];
            break;
        
        case 0x06: // SHR Vx {, Vy} - 8xy6 
            chip8->registers.V[0x0f] = chip8->registers.V[x] & 0x01; // set VF to true if LSB Vx == 1
            chip8->registers.V[x] /= 2;
            break;
        
        case 0x07: // SUBN Vx Vy - 8xy7, VF = NOT borrow
            chip8->registers.V[0x0f] = chip8->registers.V[y] > chip8->registers.V[x];
            chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
            break;

        case 0x0E: // SHL Vx {, Vy} - 8xyE
            chip8->registers.V[0x0f] = chip8->registers.V[x] & 0b10000000;
            chip8->registers.V[x] *= 2;
            break;
    }
}

static char chip8_wait_for_key(struct chip8* chip8){
    SDL_Event event;
    while(SDL_WaitEvent(&event)){
        if(event.type != SDL_KEYDOWN){
            continue;
        }

        char c = event.key.keysym.sym;
        char vkey = get_key_from_map(&chip8->keyboard, c);
        if (vkey != -1){
            return vkey;
        }
    }
    return -1;
}

static void chip8_exec_F(struct chip8* chip8, unsigned short opcode){
    unsigned char x = (opcode >> 8) & 0x000f;

    switch(opcode & 0x00ff){
        case 0x07: // LD Vx dt - fx07 Set Vx to the delay timer value
            chip8->registers.V[x] = chip8->registers.dt;
            break;
        
        case 0x0a: // LD Vx k - fx0a wait for key press, store value in Vx
        {
            char pressed = chip8_wait_for_key(chip8);
            chip8->registers.V[x] = pressed;
        }  
            break;
        
        case 0x15: // LD dt Vx - fx15 Set delay timer = Vx
            chip8->registers.dt = chip8->registers.V[x];
            break;
        
        case 0x18: // LD st Vx - fx18 Set sound timer = Vx
            chip8->registers.st = chip8->registers.V[x];
            break;
        
        case 0x1e: // ADD I Vx - fx1e I += Vx
            chip8->registers.I += chip8->registers.V[x];
            break;
        
        case 0x29: // LD F Vx - fx29 I = location of sprite for Vx
            chip8->registers.I = chip8->registers.V[x] * DEFAULT_SPRITE_HEIGHT;
            break;
        
        case 0x33: // LD B Vx - fx33 Store decimal value of Vx so 100s in I, 10s in I+1 and 1s in I+2
        {
            unsigned char hundreds = chip8->registers.V[x] / 100;
            unsigned char tens = chip8->registers.V[x] / 10 % 10;
            unsigned char ones = chip8->registers.V[x] % 10;
            set_memory(&chip8->memory, chip8->registers.I, hundreds);
            set_memory(&chip8->memory, chip8->registers.I+1, tens);
            set_memory(&chip8->memory, chip8->registers.I+2, ones);
        }
            break;
        
        case 0x55: // LD [I] Vx - fx55 Stores V0 to Vx in memory starting at location I
        {
            for (int i = 0; i <= x; i++){
                set_memory(&chip8->memory, chip8->registers.I+i, chip8->registers.V[i]);
            }
        }
            break;
        
        case 0x65: // LD Vx [I] - fx65 Read registers V0 to Vx starting at location I
        {
            for (int i = 0; i <= x; i++){
                chip8->registers.V[i] = get_memory(&chip8->memory, chip8->registers.I+i);
            }
        }
            break;
        
    }
}
static void chip8_exec_ext(struct chip8* chip8, unsigned short opcode){
    unsigned short nnn = opcode & 0x0fff;
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char kk = opcode & 0x00ff;
    unsigned char n = opcode & 0x000f;

    switch(opcode & 0xf000){
        case 0x1000: // jump to location nnn
            chip8->registers.PC = nnn;
            break;

        case 0x2000: // call subroutine at location nnn
            stack_push(chip8, chip8->registers.PC);
            chip8->registers.PC = nnn;
            break;

        case 0x3000: // SE Vx byte - 3xkk skip next instruction if Vx == kk
            if(chip8->registers.V[x] == kk){
                chip8->registers.PC += 2;
            }
            break;

        case 0x4000: // SNE Vx byte - 4xkk skip next instruction if Vx != kk
            if(chip8->registers.V[x] != kk){
                chip8->registers.PC += 2;
            }
            break;

        case 0x5000: // SE Vx Vy - 5xy0 Skip next instruction if Vx == Vy
            if (chip8->registers.V[x] == chip8->registers.V[y]){
                chip8->registers.PC += 2;
            }
            break;

        case 0x6000: // LD Vx byte - 6xkk Vx = kk
            chip8->registers.V[x] = kk;
            break;

        case 0x7000: // ADD Vx byte - 7xkk Vx = Vx + kk
            chip8->registers.V[x] += kk;
            break;
        
        case 0x8000:
            chip8_exec_eight(chip8, opcode);
            break;
        
        case 0x9000: // SNE Vx Vy - 9xy0 skip if Vx != Vy
            if(chip8->registers.V[x] != chip8->registers.V[y]){
                chip8->registers.PC += 2;
            }
            break;
        
        case 0xA000: // LD I, nnn - Annn
            chip8->registers.I = nnn;
            break;
        
        case 0xB000: // JP V0, nnn - Bnnn jump to location nnn + V0
            chip8->registers.PC = nnn + chip8->registers.V[0x00];
            break;
        
        case 0xC000: // RND Vx byte - cxkk Vx = random byte AND kk
            srand(clock());
            chip8->registers.V[x] = (rand() % 255) & kk;
            break;
        
        case 0xD000: // DRW Vx Vy nibble - Dxyn Draws sprite to screen
        {
            const char* sprite = (const char*) &chip8->memory.memory[chip8->registers.I];
            chip8->registers.V[0x0f] = draw_sprite(&chip8->screen, chip8->registers.V[x], chip8->registers.V[y],sprite, n);
        }
            break;
        
        case 0xE000:
        {
            switch(opcode & 0x00ff){
                case 0x9e: // SKP Vx - Ex9E skip next instruction if key with value of Vx is pressed
                    if(keyboard_is_down(&chip8->keyboard, chip8->registers.V[x])){
                        chip8->registers.PC += 2;
                    }
                    break;

                case 0xa1: // SKNP Vx - Exa1 skip next instruction if key is not pressed
                    if(!keyboard_is_down(&chip8->keyboard, chip8->registers.V[x])){
                        chip8->registers.PC += 2;
                    }
                    break;
            }
        }
            break;
        
        case 0xF000:
            chip8_exec_F(chip8, opcode);
            break;
    }
}

void chip8_init(struct chip8* chip8){
    memset(chip8, 0, sizeof(struct chip8));
    memcpy(&chip8->memory.memory, default_char_set, sizeof(default_char_set));
}

void chip8_exec(struct chip8* chip8, unsigned short opcode){
    switch(opcode){
        case 0x00E0: //CLS - clear screen
            screen_clear(&chip8->screen);
            break;
        case 0x00EE: //RET - return from subroutine
            chip8->registers.PC = stack_pop(chip8);
            break;
        default:
            chip8_exec_ext(chip8, opcode);
    }
}

void chip8_load(struct chip8* chip8, const char* buf, size_t size){
    assert(size + PROGRAM_LOAD_ADDR < MEMORY_SIZE);
    memcpy(&chip8->memory.memory[PROGRAM_LOAD_ADDR], buf, size);
    chip8->registers.PC = PROGRAM_LOAD_ADDR;
}