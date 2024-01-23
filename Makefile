OBJS=src/heap_hash_map.c src/count_uniq_elements.c
CFLAGS=-g -Wall -lm -std=c11 -Wno-pointer-to-int-cast -Wno-memset-elt-size -fno-omit-frame-pointer
  
CC=gcc
clear_work_dir:
	rm -r ./work_dir/*

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

valgrind: build
	valgrind ./main