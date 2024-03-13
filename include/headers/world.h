#ifndef WORLD_H
#define WORLD_H

#include <sqlite3.h>

#include "./constants.h"
#include "./camera.h"
#include "./cursor.h"

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

void generate_world(sqlite3 *db);
void load_world(SDL_Window *window, SDL_Renderer *renderer, int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_WIDTH], Camera_Object camera, Cursor_Object cursor);

void open_world(int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_WIDTH]);
void save_world(int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_WIDTH]);

#endif
