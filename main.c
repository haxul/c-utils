#include "src/heap_hash_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* stringh(char* str) {
    size_t len = strlen(str);
    char* new_str = malloc(len + 1);
    new_str[len] = '\0';
    memcpy(new_str, str, len);

    return new_str;
}


int main() {
    hash_map* hm = new_hash_map();
    if (hm == NULL) return 1;

    char* one = "1";
    char* two = "2";
    char* three = "3";

    char* hello = stringh("hello");
    char* world = stringh("world");
    char* privet = stringh("privet");


    put_hash_map(hm, one, hello);
    put_hash_map(hm, two, world);
    put_hash_map(hm, three, privet);

    printf("1 ->%s\n", (char *)get_hash_map(hm, "1"));
    printf("2 ->%s\n", (char *)get_hash_map(hm, "2"));
    printf("3 ->%s\n", (char *)get_hash_map(hm, "3"));
    free_hash_map(hm);

    return 0;
}
