#ifndef WORLD_H
#define WORLD_H

#include <sqlite3.h>
#include <SDL2/SDL_mixer.h>

#include <constants.h>

class World {
    sqlite3 *db;

    public:
        int map[MAP_HEIGHT][MAP_LENGTH][MAP_WIDTH];
        
        World(char *filename);
        
        void generate();
        void load();
        void save();

        void break_block(int x, int y, int z);
        void place_block(int x, int y, int z, int id);

        Mix_Chunk *block_break;
        Mix_Chunk *block_place;
};

#endif
