#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include <sqlite3.h>

#include "./block.h"
#include "./constants.h"
#include "./camera.h"
#include "./cursor.h"

void open_world(int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_WIDTH]) {
    sqlite3 *db;

    int rc = sqlite3_open("world.db", &db);

    if (rc) {
        printf("Error opening world: \n%s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_stmt *stmt;

    char *sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='map'";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_ROW) {
        generate_world(db);
    }
    
    sqlite3_finalize(stmt);

    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_LENGTH; x++) {
            for (int z = 0; z < WORLD_WIDTH; z++) {
                char sql[100];
                sqlite3_stmt *stmt;

                sprintf(sql, "SELECT id FROM map WHERE x=%d AND y=%d AND z=%d", x, y, z);
                
                sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

                rc = sqlite3_step(stmt);

                if (rc == SQLITE_ROW) {
                    int id = sqlite3_column_int(stmt, 0);
                    world[y][x][z] = id;
                }

                sqlite3_finalize(stmt);
            }
        }
    }

    sqlite3_close(db);
}

void generate_world(sqlite3 *db) {
    char *sql = "CREATE TABLE map (" \
                "x INTEGER, " \
                "y INTEGER, " \
                "z INTEGER, " \
                "id INTEGER );";
    
    sqlite3_exec(db, sql, NULL, 0, NULL);

    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_LENGTH; x++) {
            for (int z = 0; z < WORLD_WIDTH; z++) {
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

void draw_block(SDL_Window *window, SDL_Renderer *renderer, int x, int y, int id) {
    int width;
    int height;

    SDL_GetWindowSize(window, &width, &height);
    int real_x = (width / 2) + (TEXTURE_SIZE * x * 0.5);
    int real_y = (height / 2) - (TEXTURE_SIZE * y * 0.25);

    char *block_names[] = { "cursor", "stone", "cobblestone", "dirt", "grass", "sand", "log", "planks", "bricks", "leaves" };
    char *block_name = block_names[id];

    char path[100];
    sprintf(path, "graphics\\blocks\\%s.png", block_name);

    SDL_Surface *surface = IMG_Load(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect destination = { real_x, real_y, TEXTURE_SIZE, TEXTURE_SIZE };
    SDL_RenderCopy(renderer, texture, NULL, &destination);
    SDL_DestroyTexture(texture);
}

void load_world(SDL_Window *window, SDL_Renderer *renderer, int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_WIDTH], Camera_Object camera, Cursor_Object cursor) {
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_LENGTH; x++) {
            for (int z = 0; z < WORLD_WIDTH; z++) {
                int x_2d = x - z;
                int y_2d = 2*y - x - z;

                int diff_x_2d = x_2d - camera.x;
                int diff_y_2d = y_2d - camera.y;

                int id = world[y][x][z];

                if (id > 0) {
                    if (x_2d > camera.x - RENDER_DISTANCE && x_2d < camera.x + RENDER_DISTANCE && y_2d > camera.y - RENDER_DISTANCE && y_2d < camera.y + RENDER_DISTANCE) {
                        draw_block(window, renderer, diff_x_2d, diff_y_2d, id);
                    }
                }

                if (cursor.x == x && cursor.y == y && cursor.z == z && cursor.show) {
                    draw_block(window, renderer, diff_x_2d, diff_y_2d, CURSOR);
                }
            }
        }
    }
}

void save_world(int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_WIDTH]) {
    sqlite3 *db;
    int rc = sqlite3_open("world.db", &db);

    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_LENGTH; x++) {
            for (int z = 0; z < WORLD_WIDTH; z++) {
                int id = world[y][x][z];

                sqlite3_stmt *stmt;

                char sql[100];
                sprintf(sql, "UPDATE map SET id = %d WHERE x = %d AND y = %d AND z = %d", id, x, y, z);

                sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

                sqlite3_step(stmt);
            }
        }
    }

    sqlite3_close(db);
}
