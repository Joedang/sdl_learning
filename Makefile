# compiler
CC=g++
# compiler flags
CC_FLAGS=
# linker flags
CC_LINK=-lSDL2 -lSDL2_image
# files to compile as part of the project
OBJ_SRC=01_hello_SDL.cpp
# name of the executable
OBJ_NAME=01_hello_SDL
.DEFAULT_GOAL := main

$(OBJ_NAME): $(OBJ_SRC)
	$(CC) $(OBJ_SRC) $(CC_FLAGS) $(CC_LINK) -o $(OBJ_NAME)

%: %.cpp
	$(CC) $@.cpp $(CC_FLAGS) $(CC_LINK) -o $@

.PHONY: clean

clean:
	rm $(OBJ_NAME)
	rm *.o
