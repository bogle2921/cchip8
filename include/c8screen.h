#ifndef c8SCREEN_H
#define c8SCREEN_H

#include <stdbool.h>
#include "config.h"

struct c8_screen {
    bool pixels[SCREEN_HEIGHT][SCREEN_WIDTH];
};

void screen_set(struct c8_screen* screen, int x, int y);
bool screen_is_set(struct c8_screen* screen, int x, int y);
bool draw_sprite(struct c8_screen* screen, int x, int y, const char* sprite, int num);
void screen_clear(struct c8_screen* screen);
#endif