#include "./constants.h"
#include <stdio.h>

typedef struct {
    int x;
    int y;
} Camera_Object;

void camera_left(Camera_Object* camera) {
    camera->x--;
}

void camera_right(Camera_Object* camera) {
    camera->x++;
}

void camera_up(Camera_Object* camera) {
    camera->y++;
}

void camera_down(Camera_Object* camera) {
    camera->y--;
}


// void show_camera_coords(SDL_Renderer *renderer, Camera_Object *camera, TTF_Font *font) {
//     SDL_Color colour = { 255, 255, 255 };

//     char text[50];
//     sprintf(text, "Camera\nX:%d\nY:%d", camera.x, camera.y)

//     SDL_Surface *surface = TTF_RenderText_Solid(font, "Hello World!", colour);

//     SDL_Texture *texture =  SDL_CreateTextureFromSurface(renderer, text);

//     SDL_Rect destination = { 0, 0, surface->w, surface->h };
//     SDL_RenderCopy(renderer, texture, &destination);
// }
