#include "c8keyboard.h"
#include <assert.h>
static void keyboard_in_bounds(int key){
    assert(key >= 0 && key < TOTAL_KEYS);
}

int get_key_from_map(const char* map, char key){
    for(int i = 0; i < TOTAL_KEYS; i++){
        if(map[i] == key){
            return i;
        }
    }

    return -1;
}

void keyboard_down(struct c8_keyboard* keyboard, int key){
    keyboard->keyboard[key] = true;
}

void keyboard_up(struct c8_keyboard* keyboard, int key){
    keyboard->keyboard[key] = false;
}

bool keyboard_is_down(struct c8_keyboard* keyboard, int key){
    return keyboard->keyboard[key];
}