#include <iostream>
#include <cmath>

#include <game.h>
#include <blocks.h>

#include <SDL2/SDL_mixer.h>

using namespace std;

Game::Game(int width, int height, char *title) {
    this->width = width;
    this->height = height;
    this->title = title;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        this->sdl_window = SDL_CreateWindow(
            this->title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            this->width,
            this->height,
            SDL_WINDOW_RESIZABLE
        );

        if (sdl_window) {
            this->sdl_renderer = SDL_CreateRenderer(this->sdl_window, -1, SDL_RENDERER_ACCELERATED);

            if (sdl_renderer) {
                if (TTF_Init() == 0) {
                    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0) {
                        this->success = true;

                        world = new World("world.db");
                        world->load();

                        blocksheet = IMG_Load("textures/blocks.png");
                        texture_bs = SDL_CreateTextureFromSurface(sdl_renderer, blocksheet);

                        invsheet = IMG_Load("textures/inventory.png");
                        texture_inv = SDL_CreateTextureFromSurface(sdl_renderer, invsheet);

                        hotbar_surface = IMG_Load("textures/hotbar.png");
                        texture_hb = SDL_CreateTextureFromSurface(sdl_renderer, hotbar_surface);

                        hotbar_sel_surface = IMG_Load("textures/hotbar_selected.png");
                        texture_hbs = SDL_CreateTextureFromSurface(sdl_renderer, hotbar_sel_surface);

                        this->camera = new Camera(0, 0);
                        this->cursor = new Cursor(0, 2, 0);

                        SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
                    } else {
                        cout << "Error initialising SDL_mixer." << endl;
                        cout << Mix_GetError() << endl;
                    }
                } else {
                    cout << "Error initialising font library." << endl;
                    cout << TTF_GetError() << endl;
                }

            } else {
                cout << "Error occurred creating SDL renderer." << endl;
                cout << SDL_GetError() << endl;
            }
        } else {
            cout << "Error occurred creating SDL window." << endl;
            cout << SDL_GetError() << endl;
        }
    } else {
        cout << "Error occurred during SDL initialisation." << endl;
        cout << SDL_GetError() << endl;
    }
}

void Game::set_background(int r, int g, int b, int a) {
    this->background[0] = r;
    this->background[1] = g;
    this->background[2] = b;
    this->background[3] = a;
}

void Game::set_icon(char *path) {
    SDL_Surface *icon = IMG_Load(path);

    SDL_SetWindowIcon(sdl_window, icon);

    SDL_FreeSurface(icon);
}

void Game::update() {
    float time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - this->last_frame_time);
    
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    this->last_frame_time = SDL_GetTicks();

    SDL_SetRenderDrawColor(this->sdl_renderer, this->background[0], this->background[1], this->background[2], this->background[3]);
    SDL_RenderClear(this->sdl_renderer);

    this->render_world();

    this->show_text("Isometricraft v0.3", "Minecraft", SDL_Color { 230, 230, 230 }, 18, 10, 10);
    
    char camera_coords[50];
    sprintf(camera_coords, "Camera: X: %d, Y: %d", this->camera->fx, this->camera->fy);
    this->show_text(camera_coords, "Minecraft", SDL_Color {230, 230, 230}, 18, 10, 50);
    
    char cursor_coords[50];
    sprintf(cursor_coords, "Cursor: X: %d, Y: %d, Z: %d", this->cursor->x, this->cursor->y, this->cursor->z);
    this->show_text(cursor_coords, "Minecraft", SDL_Color {230, 230, 230}, 18, 10, 70);
    
    char gcc_version[50];
    sprintf(gcc_version, "GCC toolchain: %d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    this->show_text(gcc_version, "Minecraft", SDL_Color {230, 230, 230}, 18, this->width - 9*strlen(gcc_version), 10);

    char sdl_version[50];
    SDL_version compiled;
    SDL_VERSION(&compiled);
    sprintf(sdl_version, "SDL Version: %d.%d.%d", compiled.major, compiled.minor, compiled.patch);
    this->show_text(sdl_version, "Minecraft", SDL_Color {230, 230, 230}, 18, this->width - 9*strlen(sdl_version), 30);

    this->show_hotbar();

    if (this->inventory_open) {

        this->show_inventory();
    }

    SDL_RenderPresent(sdl_renderer);
}

void Game::draw_block(int fx, int fy, int id) {
    int rx = (this->width / 2) + (128 * fx * 0.5);
    int ry = (this->height / 2) - (128 * fy * 0.25);

    int rows = blocksheet->h / 64;
    int columns = blocksheet->w / 64;

    int bx = id;
    int by = 0;

    while (bx > columns-1) {
        by++;
        bx -= columns;
    }

    SDL_Rect clip;
    clip.w = 64;
    clip.h = 64;
    clip.x = bx * 64;
    clip.y = by * 64;
    
    SDL_Rect destination;
    destination.x = rx;
    destination.y = ry;
    destination.w = 128;
    destination.h = 128;

    SDL_RenderCopy(sdl_renderer, texture_bs, &clip, &destination);
}

void Game::render_world() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_LENGTH; x++) {
            for (int z = 0; z < MAP_WIDTH; z++) {

                int c_fx = this->camera->fx;
                int c_fy = this->camera->fy;

                int fx = x - z;
                int fy = 2*y - x - z;

                if (fx >= c_fx - RENDER_DISTANCE && fx <= c_fx + RENDER_DISTANCE) {
                    if (fy >= c_fy - RENDER_DISTANCE && fy <= c_fy + RENDER_DISTANCE) {
                        int id = this->world->map[y][x][z];

                        fx -= c_fx;
                        fy -= c_fy;

                        if (id > 0) {
                            this->draw_block(fx, fy, id);
                        }

                        if (x == this->cursor->x && y == this->cursor->y && z == this->cursor->z && this->camera->locked) {
                            this->draw_block(fx, fy, CURSOR);
                        }
                    }
                }
            }
        }
    }
}

bool Game::is_block_surrounded(int x, int y, int z) {
    bool surrounded_y = (y < MAP_HEIGHT-1) ? this->world->map[y + 1][x][z] > 0 : false;
    bool surrounded_x = (x < MAP_LENGTH-1) ? this->world->map[y][x + 1][z] > 0 : false;
    bool surrounded_z = (z < MAP_WIDTH-1) ? this->world->map[y][x][z + 1] > 0 : false;
    return surrounded_y && surrounded_x && surrounded_z;
}

void Game::show_text(char *text, char *font, SDL_Color colour, int size, int x, int y) {
    char path[50];
    sprintf(path, "fonts/%s.ttf", font);

    TTF_Font *ttf = TTF_OpenFont(path, size);

    SDL_Surface *surface = TTF_RenderText_Solid(ttf, text, colour);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);

    SDL_Rect destination = { x, y, surface->w, surface->h };
    SDL_RenderCopy(sdl_renderer, texture, NULL, &destination);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(ttf);
}

void Game::show_inventory() {
    SDL_Rect screen = { 0, 0, this->width, this->height };

    SDL_SetRenderDrawColor(sdl_renderer, 80, 80, 80, 200);
    SDL_RenderFillRect(sdl_renderer, &screen);

    int i_width = this->invsheet->w * 2;
    int i_height = this->invsheet->h * 2;

    int start_x = 0.5*this->width - 0.5*i_width;
    int start_y = 0.5*this->height - 0.5*i_height;

    SDL_Rect bg = { start_x, start_y, i_width, i_height };

    SDL_SetRenderDrawColor(this->sdl_renderer, 200, 200, 200, 255);

    SDL_RenderFillRect(this->sdl_renderer, &bg);

    int rows = invsheet->h / 32;
    int columns = invsheet->w / 32;

    for (int iy = 0; iy < rows; iy++) {
        for (int ix = 0; ix < columns; ix++) {
            SDL_Rect clip = { ix * 32, iy * 32, 32, 32 };
            SDL_Rect dest = { start_x + 64*ix + 8, start_y + 64*iy + 8, 48, 48 };
            SDL_RenderCopy(sdl_renderer, texture_inv, &clip, &dest);
        }
    }
}

void Game::handle_inv_mouseclick() {
    int mx;
    int my;

    SDL_GetMouseState(&mx, &my);

    int i_width = this->invsheet->w * 2;
    int i_height = this->invsheet->h * 2;

    int start_x = 0.5*this->width - 0.5*i_width;
    int start_y = 0.5*this->height - 0.5*i_height;

    mx -= start_x;
    my -= start_y;

    int i_x = floor(mx / 64);
    int i_y = floor(my / 64);

    int rows = invsheet->h / 32;
    int columns = invsheet->w / 32;

    int temp_selection = i_y*columns + i_x + 1;

    if (temp_selection <= rows * columns) {
        this->hotbar[this->selected_slot] = temp_selection;
        this->inventory_open = false;
    }
}

void Game::show_hotbar() {
    int hb_width = this->hotbar_surface->w;
    int hb_height = this->hotbar_surface->h;

    int start_x = 0.5*this->width - 0.5*hb_width;
    int start_y = this->height - 1.2*hb_height;

    SDL_Rect dest = { start_x, start_y, hb_width, hb_height };

    SDL_RenderCopy(this->sdl_renderer, this->texture_hb, NULL, &dest);

    int rows = invsheet->h / 32;
    int columns = invsheet->w / 32;

    for (int i = 0; i < 9; i++) {
        int id = this->hotbar[i];

        int count = 0;

        int ix;
        int iy;

        bool found = false;

        for (iy = 0; iy < rows; iy++) {
            for (ix = 0; ix < columns; ix++) {
                if (count == id - 1) {
                    found = true;
                    break;
                }

                count++;
            }
            if (found) {
                break;
            }
        }

        SDL_Rect clip = { ix * 32, iy * 32, 32, 32 };

        SDL_Rect dest = { start_x + (i * 39) + 1, start_y + 1, 38, 38 };

        SDL_RenderCopy(sdl_renderer, texture_inv, &clip, &dest);
    }

    SDL_Rect rect2 = { start_x + (this->selected_slot * 39) - 1, start_y - 1, 42, 42 };

    SDL_RenderCopy(sdl_renderer, texture_hbs, NULL, &rect2);

}

void Game::handle_resize() {
    SDL_GetWindowSize(sdl_window, &this->width, &this->height);
}
