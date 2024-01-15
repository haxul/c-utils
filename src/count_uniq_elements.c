#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define WORK_DIR "./work_dir"
#define CHUNK_SIZE 5
#define BUF_LINE_SIZE 10

int compare(const void* a, const void* b) {
    const int int_a = *((int *)a);
    const int int_b = *((int *)b);

    if (int_a == int_b) return 0;
    else if (int_a < int_b) return -1;
    else return 1;
}

void merge_files(const char* dir) {
    //TODO
}

uint32_t save_to_file(const char* workdir, const int* arr,
                      const uint32_t arr_size, const uint32_t postfix) {
    const char chunk_file_path[50];
    sprintf(chunk_file_path, "%s/file_%d", workdir, postfix);

    FILE* new_chunk_file = fopen(chunk_file_path, "wa");

    for (size_t i = 0; i < arr_size; ++i) {
        if (i == arr_size - 1) {
            fprintf(new_chunk_file, "%d", arr[i]);
        }
        else {
            fprintf(new_chunk_file, "%d\n", arr[i]);
        }
    }
    fclose(new_chunk_file);

    return 0;
}

uint64_t count_uniq_elements(const char* const file_path) {
    // check if work dir exists
    struct stat st = {0};
    stat(WORK_DIR, &st);
    if (st.st_mode == 0) {
        // create work dir
        mkdir(WORK_DIR, 0700);
    }

    // open file
    FILE* file = fopen(file_path, "r");
    if (file == NULL) return 1;

    // prepare buffers for reading from file
    int chunk_buf[CHUNK_SIZE];
    char buf_line[BUF_LINE_SIZE];
    memset(chunk_buf, 0, CHUNK_SIZE);
    memset(buf_line, 0, BUF_LINE_SIZE);

    // read a chunk of big file, sort it and store it to file system
    uint32_t chunk_idx = 0, file_count = 0;
    while (fgets(buf_line, BUF_LINE_SIZE, file)) {
        chunk_buf[chunk_idx] = atoi(buf_line);
        chunk_idx++;
        if (chunk_idx >= CHUNK_SIZE) {
            qsort(chunk_buf, CHUNK_SIZE, sizeof(int), compare);
            save_to_file(WORK_DIR, chunk_buf, CHUNK_SIZE, file_count++);
            chunk_idx = 0;
            memset(chunk_buf, 0, CHUNK_SIZE);
        }
    }

    if (chunk_idx != 0) {
        const uint32_t len = chunk_idx;
        qsort(chunk_buf, len, sizeof(int), compare);
        save_to_file(WORK_DIR, chunk_buf, len, file_count);
    }

    // recursevly merge files
    merge_files(WORK_DIR);

    // count unique elements in big sorted files
    // TODO
    fclose(file);
    return 0;
}