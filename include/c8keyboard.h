#ifndef C8KEYBOARD_H
#define C8KEYBOARD_H

#include <stdbool.h>
#include "config.h"

struct c8_keyboard {
    bool keyboard[TOTAL_KEYS];
};

int get_key_from_map(const char* map, char key);
void keyboard_down(struct c8_keyboard* keyboard, int key);
void keyboard_up(struct c8_keyboard* keyboard, int key);
bool keyboard_is_down(struct c8_keyboard* keyboard, int key);

#endif