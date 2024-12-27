#ifndef C8KEYBOARD_H
#define C8KEYBOARD_H

#include <stdbool.h>
#include "config.h"

struct c8_keyboard {
    bool keyboard[TOTAL_KEYS];
    const char* key_map;
};

void set_keyboard_map(struct c8_keyboard* keyboard, const char* map);
int get_key_from_map(struct c8_keyboard* keyboard, char key);
void keyboard_down(struct c8_keyboard* keyboard, int key);
void keyboard_up(struct c8_keyboard* keyboard, int key);
bool keyboard_is_down(struct c8_keyboard* keyboard, int key);

#endif