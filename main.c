#include "src/heap_hash_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    hash_map* hm = new_hash_map();
    if (hm == NULL) return 1;

    char* one = "one";
    char* two = "two";

    char* hello = malloc(6);
    char* world = malloc(6);
    char* privet = malloc(7);

    memcpy(hello, &"hello", 6);
    memcpy(world, &"world", 6);
    memcpy(privet, &"privet", 7);
    const uint32_t r = put_hash_map(hm, one, hello);
    const uint32_t r2 = put_hash_map(hm, two, world);
    const uint32_t r3 = put_hash_map(hm, one, privet);


    if (r != 0 || r2 != 0 || r3 != 0) {
        perror("cannot put hashmap\n");
        return 1;
    }

    char* val_one = get_hash_map(hm, one);
    char* val_two = get_hash_map(hm, two);
    printf("one -> %s\n", val_one);
    printf("two -> %s\n", val_two);

    remove_hash_map(hm);
    //char* value = get_hash_map(hm, k);

    //printf("val %s\n", value);
    return 0;
}
