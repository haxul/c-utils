#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include <stdint.h>

#define MAP_INIT_SIZE 1

typedef struct entry_node {
    void* val;
    char* key;
    struct entry_node* next;
} entry_node;

typedef struct hash_map {
    struct entry_node* bucket_arr;
    uint32_t entries_size;
    uint32_t used_buckets;
    uint32_t buckets_size;
} hash_map;

struct hash_map* new_hash_map();

void free_hash_map(struct hash_map* hm);

void* get_hash_map(struct hash_map* hm, char* key);

void remove_key_hash_map(struct hash_map* hm, char* key);

/*
 * hm - pointer to hashmap
 * key - string key for hashmap. Hashmap copys this key in heap
 * val - value. Value must be allocated in heap.
 *       After usage hashmap frees memory value uses
 */
uint32_t put_hash_map(struct hash_map* hm, char* key, void* val);

#endif
