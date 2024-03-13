all:
	gcc -g -Iinclude -Iinclude/SDL2 -Iinclude/headers -Llib -o game src/*.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lsqlite3
