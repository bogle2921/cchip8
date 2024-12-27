#include "c8screen.h"
#include <assert.h>
#include <memory.h>

static void check_bounds(int x, int y){
    assert(x >= 0 && x < SCREEN_WIDTH && y >=0 && y < SCREEN_HEIGHT);
}
void screen_set(struct c8_screen* screen, int x, int y){
    check_bounds(x, y);
    screen->pixels[y][x] = true;
}
bool screen_is_set(struct c8_screen* screen, int x, int y){
    check_bounds(x, y);
    return screen->pixels[y][x];
}

bool draw_sprite(struct c8_screen* screen, int x, int y, const char* sprite, int num){
    bool pixel_collision = false;

    for(int ly = 0; ly < num; ly++){
        char c = sprite[ly];
        for(int lx = 0; lx < 8; lx++){
            if((c & (0b10000000 >> lx)) == 0) {
                continue;
            }
            if(screen->pixels[(ly+y) % SCREEN_HEIGHT][(lx+x) % SCREEN_WIDTH]){
                pixel_collision = true;
            }
            screen->pixels[(ly+y) % SCREEN_HEIGHT][(lx+x) % SCREEN_WIDTH] ^= true;
        }
    }
    return pixel_collision;
}

void screen_clear(struct c8_screen* screen){
    memset(screen->pixels, 0, sizeof(screen->pixels));
}