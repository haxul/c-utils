#include "heap_hash_map.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


entry_node* create_entry_node(const char* const key, void* val) {
    entry_node* const new_entry_node = malloc(sizeof(entry_node));
    if (new_entry_node == NULL) {
        return NULL;
    }
    const int len = strlen(key) + 1;

    char* new_key = malloc(len * sizeof(char));
    memcpy(new_key, key, len);

    new_entry_node->key = new_key;
    new_entry_node->val = val;
    new_entry_node->next = NULL;

    return new_entry_node;
}


bool equals(const char* k1, const char* k2) {
    if (k1 == k2) {
        return true;
    }

    if (k1 == NULL || k2 == NULL) {
        return false;
    }

    const size_t k1_len = strlen(k1);
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

uint32_t hashcode(char* key) { return abs((int)key); }

hash_map* new_hash_map() {
    hash_map* hm = calloc(1, sizeof(struct hash_map));

    if (hm == NULL) {
        return NULL;
    }
    entry_node* en = calloc(MAP_INIT_SIZE, sizeof(struct entry_node));
    if (en == NULL) {
        return NULL;
    }

    hm->bucket_arr = en;
    hm->size = MAP_INIT_SIZE;
    hm->used = 0;

    return hm;
}


void remove_hash_map(hash_map* hm) {
    if (hm == NULL) {
        return;
    }

    if (hm->bucket_arr == NULL) {
        free(hm);
        return;
    }

    for (int i = 0; i < MAP_INIT_SIZE; ++i) {
        entry_node* cur_node = hm->bucket_arr[i].next;
        while (cur_node != NULL) {
            entry_node* t = cur_node->next;
            free(cur_node->key);
            free(cur_node->val);
            free(cur_node);
            cur_node = t;
        }
    }

    free(hm->bucket_arr);
    free(hm);
}


uint32_t put_hash_map(hash_map* hm, char* key, void* val) {
    if (val == NULL || key == NULL) {
        return 1;
    }

    const int target_idx = hashcode(key) % hm->size;
    entry_node* const head = &hm->bucket_arr[target_idx];
    entry_node* cur_node = head->next;

    if (cur_node == NULL) {
        // no nodes are inserted in this bucket
        hm->used++;
        cur_node = create_entry_node(key, val);
        if (cur_node == NULL) {
            return 3;
        }
        head->next = cur_node;

        return 0;
    }

    while (cur_node != NULL) {
        if (equals(cur_node->key, key)) {
            // free current value for this key
            free(cur_node->val);
            // assign new val
            cur_node->val = val;
            return 0;
        }
        else {
            cur_node = cur_node->next;
        }
    }

    // entry_node is not found, so let's insert new one in the bucket
    entry_node* new_entry_node = create_entry_node(key, val);
    entry_node* tmp = head->next;
    head->next = new_entry_node;
    new_entry_node->next = tmp;

    return 0;
}


void* get_hash_map(struct hash_map* hm, char* key) {
    if (hm == NULL || key == NULL || hm->bucket_arr == NULL) {
        return NULL;
    }
    const uint32_t hc = hashcode(key);
    const int idx = hc % hm->size;
    const entry_node* cur_node = hm->bucket_arr[idx].next;
    while (cur_node != NULL) {
        if (equals(key, cur_node->key)) {
            return cur_node->val;
        }
        cur_node = cur_node->next;
    }

    return NULL;
}
