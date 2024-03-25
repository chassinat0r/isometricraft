#include <iostream>

#include <camera.h>

using namespace std;

Camera::Camera(int fx, int fy, bool locked) {
    this->fx = fx;
    this->fy = fy;
    this->locked = locked;
}

void Camera::move_up() {
    this->fy++;
}

void Camera::move_down() {
    this->fy--;
}

void Camera::move_left() {
    this->fx--;
}

void Camera::move_right() {
    this->fx++;
}
