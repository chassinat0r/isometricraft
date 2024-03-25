all:
	g++ -g -Iinclude -Iinclude/headers -Llib -o game src/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lsqlite3
