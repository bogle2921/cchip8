#include "c8keyboard.h"
#include <assert.h>
static void keyboard_in_bounds(int key){
    assert(key >= 0 && key < TOTAL_KEYS);
}

int get_key_from_map(struct c8_keyboard* keyboard, char key){
    for(int i = 0; i < TOTAL_KEYS; i++){
        if(keyboard->key_map[i] == key){
            return i;
        }
    }

    return -1;
}

void set_keyboard_map(struct c8_keyboard* keyboard, const char* map){
    keyboard->key_map = map;
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