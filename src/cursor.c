#include <stdbool.h>

#include <SDL.h>

#include "./world.h"
#include "./block.h"
#include "./camera.h"

typedef struct {
    int x;
    int y;
    int z;
    bool show;
} Cursor_Object;

void show_cursor(SDL_Renderer *renderer, Cursor_Object cursor, Camera_Object camera) {
    int x = cursor.x;
    int y = cursor.y;
    int z = cursor.z;

    int x_2d = x - z;
    int y_2d = 2*y - x - z;

    int diff_x_2d = x_2d - camera.x;
    int diff_y_2d = y_2d - camera.y;

    draw_block(renderer, diff_x_2d, diff_y_2d, CURSOR);
}

void cursor_forward(Cursor_Object *cursor) {
    cursor->z--;
}

void cursor_backward(Cursor_Object *cursor) {
    cursor->z++;
}
