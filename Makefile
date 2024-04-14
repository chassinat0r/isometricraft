FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lsqlite3

ifeq ($(OS),Windows_NT)
	FLAGS += -lmingw32 -Llib
endif

all:
	g++ -g -Iinclude -Iinclude/headers -o game src/*.cpp $(FLAGS)
