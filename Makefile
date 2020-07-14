CC=g++
CC_FLAGS=
CC_LINK=
# files to compile as part of the project
OBJS=main.cpp
# name of the executable
OBJ_NAME=main

$(OBJ_NAME): $(OBJS)
	$(CC) $(OBJS) -o $(OBJ_NAME)

.PHONY: clean

clean:
	rm *.o
