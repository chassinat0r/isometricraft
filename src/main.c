#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "./constants.h"
#include "./display.h"
#include "./camera.h"
#include "./block.h"
#include "./world.h"
#include "./cursor.h"

bool running = false;
float last_frame_time = 0;
Camera_Object camera = { 0, 0 };
Cursor_Object cursor = { 0, 3, 0, false };
bool is_camera = true;

int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_WIDTH];

TTF_Font *font;

int selected_block = 1;

void show_selected_block(SDL_Renderer *renderer) {
    char *block_names[] = { "cursor", "stone", "cobblestone", "dirt", "grass", "sand", "log", "planks", "bricks", "leaves" };
    char *block_name = block_names[selected_block];

    char path[100];
    sprintf(path, "graphics\\blocks\\%s.png", block_name);

    SDL_Surface *surface = IMG_Load(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect destination = { WINDOW_WIDTH - 0.5*TEXTURE_SIZE - 10, 10, TEXTURE_SIZE / 2, TEXTURE_SIZE / 2};
    SDL_RenderCopy(renderer, texture, NULL, &destination);
}

void update(SDL_Renderer *renderer) {
    float time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    last_frame_time = SDL_GetTicks();

    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);

    load_world(renderer, world, camera);

    if (cursor.show) {
        show_cursor(renderer, cursor, camera);
    }

    show_text(renderer, "Isometricraft v0.1", font, (SDL_Color) {230, 230, 230}, 10, 10);

    char camera_coords[50];
    sprintf(camera_coords, "Camera: X: %d, Y: %d", camera.x, camera.y);
    show_text(renderer, camera_coords, font, (SDL_Color) {230, 230, 230}, 10, 50);


    char cursor_coords[50];
    sprintf(cursor_coords, "Cursor: X: %d, Y: %d, Z: %d", cursor.x, cursor.y, cursor.z);
    show_text(renderer, cursor_coords, font, (SDL_Color) {230, 230, 230}, 10, 70);

    show_selected_block(renderer);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    SDL_Window *window = create_window("Isometricraft");
    if (!window) {
        printf("Error occurred creating SDL window.\n");
        return;
    }
    
    SDL_Renderer *renderer = get_renderer(window);

    if (!renderer) {
        printf("Error loading SDL renderer.\n");
        return;
    }

    set_icon(window, "graphics\\icon.png");
    if (TTF_Init() < 0) {
        printf("Error initializing SDL_ttf.\n");
        return;
    }
    
    font = TTF_OpenFont("Minecraft.ttf", 18);


    generate_world(world);

    // world[2][0][0] = BRICKS;
    // world[2][0][1] = COBBLESTONE;
    // world[2][1][1] = LOG;
    // world[2][1][0] = PLANKS;
    // world[2][2][1] = SAND;
    // world[2][2][0] = STONE;
    // world[2][3][0] = LEAVES;

    running = true;

    while (running) {
        update(renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch (event.button.button) {
                        case SDL_BUTTON_LEFT:
                            if (!is_camera) {
                                world[cursor.y][cursor.x][cursor.z] = -1;
                            }
                            break;
                        case SDL_BUTTON_RIGHT:
                            if (!is_camera) {
                                world[cursor.y][cursor.x][cursor.z] = selected_block;
                            }
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        case SDLK_w:
                            if (is_camera) {
                                camera.y++;
                            } else {
                                cursor.z--;
                            }
                            break;
                        case SDLK_s:
                            if (is_camera) {
                                camera.y--;
                            } else {
                                cursor.z++;
                            }
                            break;
                        case SDLK_a:
                            if (is_camera) {
                                camera.x--;
                            } else {
                                cursor.x--;
                            }
                            break;
                        case SDLK_d:
                            if (is_camera) {
                                camera.x++;
                            } else {
                                cursor.x++;
                            }
                            break;
                        case SDLK_SPACE:
                            if (!is_camera) {
                                cursor.y++;
                            }
                            break;
                        case SDLK_LSHIFT:
                        case SDLK_RSHIFT:
                            if (!is_camera) {
                                cursor.y--;
                            }
                            break;
                        case SDLK_c:
                            is_camera = !is_camera;
                            cursor.show = !cursor.show;
                            break;
                        case SDLK_1:
                            selected_block = 1;
                            break;
                        case SDLK_2:
                            selected_block = 2;
                            break;
                        case SDLK_3:
                            selected_block = 3;
                            break;
                        case SDLK_4:
                            selected_block = 4;
                            break;
                        case SDLK_5:
                            selected_block = 5;
                            break;
                        case SDLK_6:
                            selected_block = 6;
                            break;
                        case SDLK_7:
                            selected_block = 7;
                            break;
                        case SDLK_8:
                            selected_block = 8;
                            break;      
                        case SDLK_9:
                            selected_block = 9;
                            break;                
                    break;
                    }
            }
        }
    }
}
