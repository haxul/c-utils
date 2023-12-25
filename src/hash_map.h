#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include <stdint.h>

#define MAP_INIT_SIZE 16

struct entry_node {
  void *val;
  struct entry_node *next;
};

struct hash_map {
  struct entry_node array[MAP_INIT_SIZE];
};

void *get_hash_map(struct hash_map *hm, char *key);

void put_hash_map(struct hash_map *hm, char *key, void *val);

bool equals(char *key1, char *key2);

uint64_t hashcode(char *key);

#endif