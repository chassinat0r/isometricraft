#ifndef WORLD_H
#define WORLD_H

#include "./constants.h"
#include "./camera.h"

typedef struct {
    int x;
    int y;
} Coords2D_Object;

typedef struct {
    int x;
    int y;
    int z;
} Coords3D_Object;

typedef struct {
    int x;
    int y;
} RealCoords_Object;

void generate_world(int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_WIDTH]);
void load_world(SDL_Renderer *renderer, int world[WORLD_HEIGHT][WORLD_LENGTH][WINDOW_WIDTH], Camera_Object camera);

#endif
