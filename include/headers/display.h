#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <SDL_ttf.h>

#include "./camera.h"

SDL_Window *create_window(char *title);
SDL_Renderer *get_renderer(SDL_Window *window);

void show_text(SDL_Renderer *renderer, char *text, TTF_Font *font, SDL_Color colour, int x, int y);

void set_icon(SDL_Window *window, char *path);

#endif
