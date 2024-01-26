#include "src/heap_hash_map.h"
#include "src/count_uniq_elements.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define FILE_PATH "/home/haxul/Development/clanguage/hash-map/test_data/nums.txt"

char* stringh(char* str) {
    size_t len = strlen(str);
    char* new_str = malloc(len + 1);
    new_str[len] = '\0';
    memcpy(new_str, str, len);

    return new_str;
}

uint32_t generate_file(const char* file_path) {
    srand(time(NULL));
    uint32_t count = 0;
    const uint32_t size = 100;
    FILE* file = fopen(file_path, "wa");
    if (file == NULL) {
        perror("cannot open file\n");
        return 1;
    }
    while (count < size) {
        fprintf(file, "%d\n", rand() % 100);
        count += 1;
    }

    return 0;
}


int main() {
    uint64_t res = count_uniq_elements(FILE_PATH);
    printf("result %lu\n", res);

    return 0;
}
