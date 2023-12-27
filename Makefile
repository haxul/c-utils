OBJS=src/heap_hash_map.c
CFLAGS=-g -Wall -std=c11 -Wno-pointer-to-int-cast
  
CC=gcc

all: build

debug: build gdb-start

gdb-start:
	gdb main -tui

build:
	$(CC) $(CFLAGS) $(OBJS) main.c -o main