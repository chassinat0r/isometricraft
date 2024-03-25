#ifndef CAMERA_H
#define CAMERA_H

class Camera {
    public:
        int fx;
        int fy;
        bool locked;

        Camera(int fx, int fy, bool locked = false);

        void move_up();
        void move_down();
        void move_left();
        void move_right();
};

#endif
