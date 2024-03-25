#include <iostream>
#include <SDL2/SDL.h>

#include <game.h>
#include <camera.h>
#include <world.h>

using namespace std;

bool running = false;

int main(int argv, char** args) {
    Game myGame(1280, 720, "Isometricraft");

    if (myGame.success) {
        myGame.set_background(135, 206, 235, 255);
        myGame.set_icon("textures/icon.png");
        running = true;
    }

    while (running) {
        myGame.update();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            myGame.handle_resize();
                            break;
                    }
                    break;
                case SDL_QUIT:
                    running = false;
                    myGame.world->save();
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch (event.button.button) {
                        case SDL_BUTTON_LEFT:
                            if (!myGame.inventory_open && myGame.camera->locked) {
                                myGame.world->break_block(myGame.cursor->x, myGame.cursor->y, myGame.cursor->z);
                            }
                            if (myGame.inventory_open) {
                                myGame.handle_inv_mouseclick();
                            }
                            break;
                        case SDL_BUTTON_RIGHT:
                            if (!myGame.inventory_open && myGame.camera->locked) {
                                myGame.world->place_block(myGame.cursor->x, myGame.cursor->y, myGame.cursor->z, myGame.hotbar[myGame.selected_slot]);
                            }
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            if (myGame.inventory_open) {
                                myGame.inventory_open = false;
                            } else {
                                running = false;
                                myGame.world->save();
                            }
                            break;
                        case SDLK_w:
                            if (!myGame.inventory_open) {
                                if (myGame.camera->locked) {
                                    myGame.cursor->move(0, 0, 1);
                                } else {
                                    myGame.camera->move_up();
                                }
                            }
                            break;
                        case SDLK_s:
                            if (!myGame.inventory_open) {
                                if (myGame.camera->locked) {
                                    myGame.cursor->move(0, 0, -1);
                                } else {
                                    myGame.camera->move_down();
                                }
                            }
                            break;
                        case SDLK_a:
                            if (!myGame.inventory_open) {
                                if (myGame.camera->locked) {
                                    myGame.cursor->move(-1, 0, 0);
                                } else {
                                    myGame.camera->move_left();
                                }
                            }
                            break;
                        case SDLK_d:
                            if (!myGame.inventory_open) {
                                if (myGame.camera->locked) {
                                    myGame.cursor->move(1, 0, 0);
                                } else {
                                    myGame.camera->move_right();
                                }
                            }
                            break;
                        case SDLK_SPACE:
                            if (myGame.camera->locked && !myGame.inventory_open) {
                                myGame.cursor->move(0, 1, 0);
                            }
                            break;
                        case SDLK_LSHIFT:
                        case SDLK_RSHIFT:
                            if (myGame.camera->locked && !myGame.inventory_open) {
                                myGame.cursor->move(0, -1, 0);
                            }
                            break;
                        case SDLK_c:
                            myGame.camera->locked = !myGame.camera->locked;
                            break;
                        case SDLK_e:
                            myGame.inventory_open = !myGame.inventory_open;
                            break;
                        case SDLK_1:
                            myGame.selected_slot = 0;
                            break;
                        case SDLK_2:
                            myGame.selected_slot = 1;
                            break;                                              
                        case SDLK_3:
                            myGame.selected_slot = 2;
                            break;
                        case SDLK_4:
                            myGame.selected_slot = 3;
                            break;
                        case SDLK_5:
                            myGame.selected_slot = 4;
                            break;
                        case SDLK_6:
                            myGame.selected_slot = 5;
                            break;                                              
                        case SDLK_7:
                            myGame.selected_slot = 6;
                            break;
                        case SDLK_8:
                            myGame.selected_slot = 7;
                            break;
                        case SDLK_9:
                            myGame.selected_slot = 8;
                            break;  
                    }
                    break;
            }
        }
    }

    return 0;
}
