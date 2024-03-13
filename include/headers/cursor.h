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

#endif
