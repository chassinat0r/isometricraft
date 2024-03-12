#ifndef BLOCK_H
#define BLOCK_H

#include <stdbool.h>

typedef struct {
    int x;
    int y;
    int z;
    int id;
} Block_Object;

#define CURSOR 0
#define STONE 1
#define COBBLESTONE 2
#define DIRT 3
#define GRASS 4
#define SAND 5
#define LOG 6
#define PLANKS 7
#define BRICKS 8
#define LEAVES 9
#define WATER 10

#endif
