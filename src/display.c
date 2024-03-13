#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "./constants.h"
#include "./camera.h"

SDL_Window *create_window(char *title) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return false;
    }

    return SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
}

SDL_Renderer *get_renderer(SDL_Window *window) {
    return SDL_CreateRenderer(window, -1, 0);
}

void show_text(SDL_Renderer *renderer, char *text, TTF_Font *font, SDL_Color colour, int x, int y) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, colour);

    SDL_Texture *texture =  SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect destination = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &destination);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void set_icon(SDL_Window *window, char *path) {
    SDL_Surface *surface = IMG_Load(path);

    SDL_SetWindowIcon(window, surface);

    SDL_FreeSurface(surface);
}
