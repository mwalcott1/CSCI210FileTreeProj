CC=gcc
CFLAGS=-g -Wall -std=c11
DEPS = types.h
OBJ = otherCommands.o mkdirSplitpath.o 

.DEFAULT_GOAL := main

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.c $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f mkdirSplitpath.o main

build:
	rm -f mkdirSplitpath.o main j.txt
	gcc -c mkdirSplitpath.c
	gcc -o main main.o mkdirSplitpath.o otherCommands.o
	touch j.txt
