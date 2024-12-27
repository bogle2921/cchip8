#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include "SDL2/SDL.h"
#include "chip8.h"
#include "c8keyboard.h"

const char keyboard_map[TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7,
    SDLK_8, SDLK_9, SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f
};
int main(int argc, char **argv) {
    if(argc < 2){
        printf("You must provide file to load\n");
        return -1;
    }

    const char* filename = argv[1];
    printf("Loading %s\n", filename);

    FILE* f = fopen(filename, "rb");
    if(!f){
        printf("Failed to open file\n");
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char buf[size];
    int res = fread(buf, size, 1, f);
    if(res != 1){
        printf("Failed to read from file");
        return -1;
    }

    struct chip8 chip8;
    chip8_init(&chip8);
    chip8_load(&chip8, buf, sizeof(buf));
    set_keyboard_map(&chip8.keyboard, keyboard_map);
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        EMU_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH * WINDOW_MULTI,
        SCREEN_HEIGHT * WINDOW_MULTI,
        SDL_WINDOW_SHOWN);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    while(1){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    goto out;
                    break;
                case SDL_KEYDOWN:
                {
                    char key = event.key.keysym.sym;
                    int vkey = get_key_from_map(&chip8.keyboard, key);
                    if (vkey != -1){
                        keyboard_down(&chip8.keyboard, vkey);
                    }
                }   
                    break;
                case SDL_KEYUP:
                {
                    char key = event.key.keysym.sym;
                    int vkey = get_key_from_map(&chip8.keyboard, key);
                    if (vkey != -1){
                        keyboard_up(&chip8.keyboard, vkey);
                    }
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255,255,255,0);

        for(int x = 0; x < SCREEN_WIDTH; x++){
            for(int y = 0; y < SCREEN_HEIGHT; y++){
                if(screen_is_set(&chip8.screen, x, y)){
                   SDL_Rect r;
                    r.x = x * WINDOW_MULTI;
                    r.y = y * WINDOW_MULTI;
                    r.w = WINDOW_MULTI;
                    r.h = WINDOW_MULTI;
                    SDL_RenderFillRect(renderer, &r); 
                }
            }
        }
        
        SDL_RenderPresent(renderer);

        if(chip8.registers.dt > 0){
            Sleep(100);
            chip8.registers.dt -= 1;
            printf("delay\n");
        }
        if(chip8.registers.st > 0){
            //Beep(15000, 100 * chip8.registers.st);
            chip8.registers.st = 0;
        }

        unsigned short opcode = get_memory_short(&chip8.memory, chip8.registers.PC);
        chip8.registers.PC += 2;
        chip8_exec(&chip8, opcode);
    }

out:
    SDL_DestroyWindow(window);
    return 0;
}