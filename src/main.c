#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <sqlite3.h>

#include "./constants.h"
#include "./display.h"
#include "./camera.h"
#include "./block.h"
#include "./world.h"
#include "./cursor.h"

bool running = false;
float last_frame_time = 0;
Camera_Object camera = { 0, -WORLD_HEIGHT / 2 };
Cursor_Object cursor = { WORLD_LENGTH / 2, 2, WORLD_WIDTH / 2, false };
bool is_camera = true;

SDL_Window *window;
SDL_Renderer *renderer;

int world[WORLD_HEIGHT][WORLD_LENGTH][WORLD_WIDTH];

TTF_Font *font;

int selected_block = 1;

void show_hotbar() {
    int s_width;
    int s_height;

    SDL_GetWindowSize(window, &s_width, &s_height);

    SDL_Surface* surface = IMG_Load("graphics/ui/hotbar.png"); 
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); 
    SDL_FreeSurface(surface);

    int x = 0.5*s_width - 176;
    int y = s_height - 50;

    SDL_Rect destination = { x, y, 352, 40 };

    SDL_RenderCopy(renderer, texture, NULL, &destination);
    SDL_DestroyTexture(texture);

    for (int i = 1; i <= 9; i++) {
        char *block_names[] = { "cursor", "stone", "cobblestone", "dirt", "grass", "sand", "log", "planks", "bricks", "leaves" };
        char *block_name = block_names[i];

        char path[100];
        sprintf(path, "graphics\\blocks\\%s.png", block_name);

        SDL_Surface *surface = IMG_Load(path);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        
        SDL_Rect destination = { x + 4 + (i-1) * 39, y + 2, 32, 32};
        SDL_RenderCopy(renderer, texture, NULL, &destination);
        SDL_DestroyTexture(texture);
    }

    surface = IMG_Load("graphics\\ui\\hotbar_selected.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    SDL_Rect destination2 = { x + (selected_block-1) * 39, y - 1, 42, 42 };

    SDL_RenderCopy(renderer, texture, NULL, &destination2);
    SDL_DestroyTexture(texture);
}

void update() {
    float time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    last_frame_time = SDL_GetTicks();

    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);

    load_world(window, renderer, world, camera, cursor);

    show_text(renderer, "Isometricraft v0.2", font, (SDL_Color) {230, 230, 230}, 10, 10);

    char camera_coords[50];
    sprintf(camera_coords, "Camera: X: %d, Y: %d", camera.x, camera.y);
    show_text(renderer, camera_coords, font, (SDL_Color) {230, 230, 230}, 10, 50);


    char cursor_coords[50];
    sprintf(cursor_coords, "Cursor: X: %d, Y: %d, Z: %d", cursor.x, cursor.y, cursor.z);
    show_text(renderer, cursor_coords, font, (SDL_Color) {230, 230, 230}, 10, 70);

    show_hotbar();

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    window = create_window("Isometricraft");
    if (!window) {
        printf("Error occurred creating SDL window.\n");
        return;
    }
    
    renderer = get_renderer(window);

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

    open_world(world);

    running = true;

    while (running) {
        update(renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    save_world(world);
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
                            if (!is_camera && world[cursor.y][cursor.x][cursor.z] == -1) {
                                world[cursor.y][cursor.x][cursor.z] = selected_block;
                            }
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            save_world(world);
                            running = false;
                            break;
                        case SDLK_w:
                            if (is_camera) {
                                camera.y++;
                            } else {
                                if (cursor.z > 0) {
                                    cursor.z--;
                                }
                            }
                            break;
                        case SDLK_s:
                            if (is_camera) {
                                camera.y--;
                            } else {
                                if (cursor.z < WORLD_WIDTH - 1) {
                                    cursor.z++;
                                }
                            }
                            break;
                        case SDLK_a:
                            if (is_camera) {
                                camera.x--;
                            } else {
                                if (cursor.x > 0) {
                                    cursor.x--;
                                }
                            }
                            break;
                        case SDLK_d:
                            if (is_camera) {
                                camera.x++;
                            } else {
                                if (cursor.x < WORLD_LENGTH - 1) {
                                    cursor.x++;
                                }
                            }
                            break;
                        case SDLK_SPACE:
                            if (!is_camera && cursor.y < WORLD_HEIGHT-1) {
                                cursor.y++;
                            }
                            break;
                        case SDLK_LSHIFT:
                        case SDLK_RSHIFT:
                            if (!is_camera && cursor.y > 0) {
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
