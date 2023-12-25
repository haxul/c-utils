#include "hash_map.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void *get_hash_map(struct hash_map *hm, char *key) { return NULL; }

void put_hash_map(struct hash_map *hm, char *key, void *val) {}

bool equals(char *k1, char *k2) {
  if (k1 == k2) {
    return true;
  }

  if (k1 == NULL || k2 == NULL) {
    return false;
  }

  size_t k1_len = strlen(k1);
  if (k1_len != strlen(k2)) {
    return false;
  }

  for (size_t i = 0; i < k1_len; i++) {
    if (k1[i] != k2[i]) {
      return false;
    }
  }

  return true;
}

uint64_t hashcode(char *key) { return (uint64_t)key; }