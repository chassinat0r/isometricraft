#ifndef CAMERA_H
#define CAMERA_H

typedef struct {
    int x;
    int y;
} Camera_Object;

void camera_left(Camera_Object* camera);
void camera_right(Camera_Object* camera);
void camera_up(Camera_Object* camera);
void camera_down(Camera_Object* camera);

#endif
