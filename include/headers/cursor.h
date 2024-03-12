#ifndef CURSOR_H
#define CURSOR_H

#include <stdbool.h>
#include "./camera.h"

typedef struct {
    int x;
    int y;
    int z;
    bool show;
} Cursor_Object;

void show_cursor(SDL_Renderer *renderer, Cursor_Object cursor, Camera_Object camera);

void cursor_forward(Cursor_Object *cursor);
void cursor_backward(Cursor_Object *cursor);

#endif
