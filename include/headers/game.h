#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <camera.h>
#include <constants.h>
#include <world.h>
#include <cursor.h>

class Game {
    public:
        bool success = false;

        Camera *camera;
        Cursor *cursor;
        World *world;

        Game(int width, int height, char *title);

        void update();
        
        void set_background(int r, int g, int b, int a);
        void set_icon(char *path);

        void render_world();
        void draw_block(int fx, int fy, int id);

        int hotbar[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        int selected_slot = 0;

        bool inventory_open = false;

        void handle_inv_mouseclick();
        void handle_resize();

    private:
        int width;
        int height;
        char* title;
        int background[4];

        SDL_Window *sdl_window;
        SDL_Renderer *sdl_renderer;

        SDL_Surface *blocksheet;
        SDL_Texture *texture_bs;

        SDL_Surface *invsheet;
        SDL_Texture *texture_inv;

        SDL_Surface *hotbar_surface;
        SDL_Texture *texture_hb;

        SDL_Surface *hotbar_sel_surface;
        SDL_Texture *texture_hbs;

        float last_frame_time = 0;

        bool is_block_surrounded(int x, int y, int z);

        void show_text(char *text, char *font, SDL_Color colour, int size, int x, int y);

        void show_inventory();
        void show_hotbar();
};      

#endif
