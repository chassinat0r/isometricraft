#include <cursor.h>
#include <constants.h>

Cursor::Cursor(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Cursor::move(int dx, int dy, int dz) {
    this->x += dx;
    this->y += dy;
    this->z += dz;
    if (this->x < 0) {
        this->x = 0;
    }
    if (this->x >= MAP_LENGTH) {
        this->x = MAP_LENGTH - 1;
    } 

    if (this->y < 0) {
        this->y = 0;
    }
    if (this->y >= MAP_HEIGHT) {
        this->y = MAP_HEIGHT - 1;
    } 

    if (this->z < 0) {
        this->z = 0;
    }
    if (this->z >= MAP_WIDTH) {
        this->z = MAP_WIDTH - 1;
    } 
}
