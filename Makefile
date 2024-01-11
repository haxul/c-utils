OBJS=src/heap_hash_map.c
CFLAGS=-g -Wall -lm -std=c11 -Wno-pointer-to-int-cast
  
CC=gcc

all: build

debug: build gdb-start

gdb-start:
	gdb main -tui

build:
	$(CC) $(CFLAGS) $(OBJS) main.c -o main

install:
	$(CC) -fPIC $(CFLAGS) -c $(OBJS) -o lib.o
	$(CC) -shared -o libHashmap.so lib.o
	rm lib.o
	mv libHashmap.so /usr/lib/

clean:
	rm /usr/lib/libHashmap.so

build-with-lib:
	$(CC) main.c $(CFLAGS) -lHashmap -lm -o main
