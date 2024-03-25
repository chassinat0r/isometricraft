#ifndef CURSOR_H
#define CURSOR_H

class Cursor {
    public:
        int x;
        int y;
        int z;

        Cursor(int x, int y, int z);
        
        void move(int dx, int dy, int dz);
};

#endif
