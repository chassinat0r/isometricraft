#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "./block.h"
#include "./constants.h"
#include "./camera.h"

void generate_world(int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_WIDTH]) {
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_LENGTH; x++) {
            for (int z = 0; z < WORLD_WIDTH; z++) {
                if (y == 1) {
                    world[y][x][z] = GRASS;
                } else if (y == 0) {
                    world[y][x][z] = DIRT;
                } else {
                    world[y][x][z] = -1;
                }            
            }
        }
    }
}

void draw_block(SDL_Renderer *renderer, int x, int y, int id) {
    int real_x = (WINDOW_WIDTH / 2) + (TEXTURE_SIZE * x * 0.5);
    int real_y = (WINDOW_HEIGHT / 2) - (TEXTURE_SIZE * y * 0.25);

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

void load_world(SDL_Renderer *renderer, int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_WIDTH], Camera_Object camera) {
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_LENGTH; x++) {
            for (int z = 0; z < WORLD_WIDTH; z++) {
                int id = world[y][x][z];
                if (id > 0) {
                    int x_2d = x - z;
                    int y_2d = 2*y - x - z;

                    if (x_2d > camera.x - RENDER_DISTANCE && x_2d < camera.x + RENDER_DISTANCE && y_2d > camera.y - RENDER_DISTANCE && y_2d < camera.y + RENDER_DISTANCE) {
                        int diff_x_2d = x_2d - camera.x;
                        int diff_y_2d = y_2d - camera.y;

                        draw_block(renderer, diff_x_2d, diff_y_2d, id);
                    }

                }
            }
        }
    }
}

void put_block(int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_WIDTH], int x, int y, int z, int id) {
    
}
