OBJS=src/hash_map.c
CFLAGS=-g -Wall -std=c99 -levent
CC=gcc

all: build

debug: build gdb-start

gdb-start:
	gdb main -tui

build:
	$(CC) $(CFLAGS) $(OBJS) main.c -o main