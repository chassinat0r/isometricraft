#include <iostream>

#include <world.h>
#include <blocks.h>

using namespace std;

World::World(char *filename) {
    int rc = sqlite3_open(filename, &db);

    if (rc) {
        cout << "Error opening world: " << sqlite3_errmsg(db) << endl;
    } else {
        sqlite3_stmt *stmt;

        char *sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='map'";
        sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

        rc = sqlite3_step(stmt);

        if (rc != SQLITE_ROW) {
            this->generate();
        }
        
        sqlite3_finalize(stmt);

        block_break = Mix_LoadWAV("sounds/block_break.wav");
        block_place = Mix_LoadWAV("sounds/block_place.wav");
    }
}

void World::generate() {
    char *sql = "CREATE TABLE map (" \
                "x INTEGER, " \
                "y INTEGER, " \
                "z INTEGER, " \
                "id INTEGER );";
    
    sqlite3_exec(this->db, sql, NULL, 0, NULL);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_LENGTH; x++) {
            for (int z = 0; z < MAP_WIDTH; z++) {
                int id;
                if (y == 1) {
                    id = GRASS;
                } else if (y == 0) {
                    id = DIRT;
                } else {
                    id = -1;
                }

                char sql[100];

                sprintf(sql, "INSERT INTO map (id, x, y, z) VALUES (%d, %d, %d, %d)", id, x, y, z);
                sqlite3_exec(db, sql, NULL, 0, NULL);     
            }
        }
    }
}

void World::load() {
    int rc;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_LENGTH; x++) {
            for (int z = 0; z < MAP_WIDTH; z++) {
                char sql[100];
                sqlite3_stmt *stmt;

                sprintf(sql, "SELECT id FROM map WHERE x=%d AND y=%d AND z=%d", x, y, z);
                
                sqlite3_prepare_v2(this->db, sql, -1, &stmt, NULL);

                rc = sqlite3_step(stmt);

                if (rc == SQLITE_ROW) {
                    int id = sqlite3_column_int(stmt, 0);
                    this->map[y][x][z] = id;
                }

                sqlite3_finalize(stmt);
            }
        }
    }
}

void World::save() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_LENGTH; x++) {
            for (int z = 0; z < MAP_WIDTH; z++) {
                int id = this->map[y][x][z];
                char sql[100];

                sprintf(sql, "UPDATE map SET id = %d WHERE x = %d AND y = %d AND z = %d", id, x, y, z);
                sqlite3_exec(this->db, sql, NULL, 0, NULL);     
            }
        }
    }
}


void World::place_block(int x, int y, int z, int id) {
    if (this->map[y][x][z] == -1) {
        this->map[y][x][z] = id;
        Mix_PlayChannel(-1, block_place, 0);
    }
}

void World::break_block(int x, int y, int z) {
    if (this->map[y][x][z] > 0) {
        this->map[y][x][z] = -1;
        Mix_PlayChannel(-1, block_break, 0);
    }
}
