#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include <stdint.h>

#define MAP_INIT_SIZE 2

typedef struct entry_node {
  void *val;
  char *key;
  struct entry_node *next;
} entry_node;

typedef struct hash_map {
  struct entry_node *bucket_arr;
  uint32_t size;
  uint32_t used;
} hash_map;

struct hash_map *new_hash_map();

void remove_hash_map(struct hash_map *hm);

void *get_hash_map(struct hash_map *hm, char *key);

uint32_t put_hash_map(struct hash_map *hm, char *key, void *val);

#endif