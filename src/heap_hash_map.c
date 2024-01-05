#include "heap_hash_map.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

uint32_t _put_hash_map(hash_map* hm, char* key, void* val, entry_node* existed_node);

uint32_t _resize_hash_map(hash_map* hm, const int32_t new_size);

entry_node* _to_entry_node_list(hash_map* hm) {
    entry_node dummy_node;
    dummy_node.next = NULL;
    entry_node* cur_node;
    entry_node* t_ptr = &dummy_node;
    for (int i = 0; i < hm->buckets_size; ++i) {
        cur_node = hm->bucket_arr[i].next;
        while (cur_node != NULL) {
            t_ptr->next = cur_node;
            t_ptr = t_ptr->next;
            cur_node = cur_node->next;
        }
        hm->bucket_arr[i].next = NULL;
    }

    hm->entries_size = 0;
    hm->used_buckets = 0;
    return dummy_node.next;
}

entry_node* _create_entry_node_with_key_copy(char* key, void* val) {
    entry_node* const new_entry_node = malloc(sizeof(entry_node));
    if (new_entry_node == NULL) {
        return NULL;
    }
    const int len = strlen(key) + 1;

    char* new_key = malloc(len * sizeof(char));
    memcpy(new_key, key, len);
    new_key[len - 1] = '\0';
    new_entry_node->key = new_key;
    new_entry_node->val = val;
    new_entry_node->next = NULL;

    return new_entry_node;
}


bool _equals(const char* k1, const char* k2) {
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

uint32_t _hashcode(char* key) { return abs((int)key); }

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
    hm->entries_size = 0;
    hm->used_buckets = 0;
    hm->buckets_size = MAP_INIT_SIZE;

    return hm;
}

void remove_key_hash_map(hash_map* hm, char* key) {
    const uint32_t hc = _hashcode(key);
    const int idx = hc % hm->buckets_size;
    entry_node* prev_node = &hm->bucket_arr[idx];
    entry_node* cur_node = hm->bucket_arr[idx].next;
    while (cur_node != NULL) {
        if (_equals(cur_node->key, key)) {
            prev_node->next = cur_node->next;
            cur_node->next = NULL;
            free(cur_node->key);
            free(cur_node->val);
            free(cur_node);
            if (hm->bucket_arr[idx].next == NULL) {
                hm->used_buckets--;
            }
            hm->entries_size--;

            return;
        }
        prev_node = prev_node->next;
        cur_node = cur_node->next;
    }
}

void free_hash_map(hash_map* hm) {
    if (hm == NULL) {
        return;
    }

    if (hm->bucket_arr == NULL) {
        free(hm);
        return;
    }

    for (int i = 0; i < hm->buckets_size; ++i) {
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

uint32_t _put_hash_map(hash_map* hm, char* key, void* val, entry_node* existed_node) {
    if (val == NULL || key == NULL) {
        return 1;
    }
    const int treashold = (int)ceil(hm->buckets_size * 0.75);
    if (hm->used_buckets >= treashold) {
        _resize_hash_map(hm, hm->buckets_size * 2);
    }

    const int target_idx = _hashcode(key) % hm->buckets_size;
    entry_node* const head = &hm->bucket_arr[target_idx];
    entry_node* cur_node = head->next;

    if (cur_node == NULL) {
        // no nodes are inserted in this bucket
        hm->used_buckets++;
        hm->entries_size++;
        cur_node = existed_node == NULL ? _create_entry_node_with_key_copy(key, val) : existed_node;
        if (cur_node == NULL) {
            return 3;
        }
        head->next = cur_node;

        return 0;
    }

    while (cur_node != NULL) {
        if (_equals(cur_node->key, key)) {
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
    // or if existed node is passed as argument use it
    entry_node* new_entry_node = existed_node == NULL
                                     ? _create_entry_node_with_key_copy(key, val)
                                     : existed_node;
    entry_node* tmp = head->next;
    head->next = new_entry_node;
    new_entry_node->next = tmp;
    hm->entries_size++;

    return 0;
}

uint32_t put_hash_map(hash_map* hm, char* key, void* val) {
    return _put_hash_map(hm, key, val, NULL);
}

uint32_t _resize_hash_map(hash_map* hm, const int32_t new_size) {
    entry_node* node = _to_entry_node_list(hm);
    hm->bucket_arr = realloc(hm->bucket_arr, sizeof(entry_node) * new_size);
    if (hm->bucket_arr == NULL) {
        return -1;
    }
    memset(hm->bucket_arr, 0, sizeof(entry_node) * new_size);
    while (node != NULL) {
        _put_hash_map(hm, node->key, node->val, node);
        node = node->next;
    }

    hm->buckets_size = new_size;
    return 0;
}

void* get_hash_map(struct hash_map* hm, char* key) {
    if (hm == NULL || key == NULL || hm->bucket_arr == NULL) {
        return NULL;
    }
    const uint32_t hc = _hashcode(key);
    const int idx = hc % hm->buckets_size;
    const entry_node* cur_node = hm->bucket_arr[idx].next;
    while (cur_node != NULL) {
        if (_equals(key, cur_node->key)) {
            return cur_node->val;
        }
        cur_node = cur_node->next;
    }

    return NULL;
}
