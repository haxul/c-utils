#include "heap_hash_map.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

uint32_t hashcode(char *key) { return abs((int)key); }

struct hash_map *new_hash_map() {
  struct hash_map *hm = calloc(1, sizeof(struct hash_map));

  if (hm == NULL) {
    return NULL;
  }
  struct entry_node *en = calloc(MAP_INIT_SIZE, sizeof(struct entry_node));
  if (en == NULL) {
    return NULL;
  }

  hm->bucket_arr = en;
  hm->size = MAP_INIT_SIZE;
  hm->used = 0;

  return hm;
}
/*
void remove_hash_map(struct hash_map *hm) {
  if (hm == NULL) {
    return;
  }

  if (hm->bucket_arr == NULL) {
    free(hm);
    return;
  }

  struct entry_node *t;

  struct entry_node *cur_node = hm->bucket_arr;

  while (cur_node != NULL) {
    t = cur_node->next;
    free(cur_node->val);
    free(cur_node);
    cur_node = t;
  }

  free(hm);
}

  */
uint32_t put_hash_map(struct hash_map *hm, char *key, void *val) {
  if (val == NULL || key == NULL) {
    return 1;
  }

  int target_idx = hashcode(key) % hm->size;
  struct entry_node *prev_node = &hm->bucket_arr[target_idx];
  struct entry_node *cur_node = prev_node->next;

  if (cur_node == NULL) {
    // no nodes are inserted in this bucket
    hm->used++;
    cur_node = calloc(1, sizeof(struct entry_node));
    if (cur_node == NULL) {
      return 3;
    }
    int len = strlen(key) + 1;

    char *new_key = malloc(len * sizeof(char));
    memcpy(new_key, key, len);

    cur_node->key = new_key;
    cur_node->val = val;
    prev_node->next = cur_node;

    return 0;
  }

  /*
    while (cur_node != NULL) {
      if (equals(cur_node->key, key)) {
        cur_node->val = val;
        return -1;
      } else {
        cur_node = cur_node->next;
      }
    }
  */

  if (cur_node->val != NULL) {
    free(cur_node->val);
  }

  cur_node->val = val;

  return 0;
}
/*
void *get_hash_map(struct hash_map *hm, char *key) {
  if (hm == NULL || key == NULL || hm->bucket_arr == NULL) {
    return NULL;
  }
  uint32_t hc = hashcode(key);
  int idx = hc % hm->size;
  struct entry_node *cur_node = hm->bucket_arr;
  while (cur_node != NULL) {
    if (equals(key, cur_node->key)) {
      return cur_node->val;
    }
    cur_node = cur_node->next;
  }

  return NULL;
}
*/