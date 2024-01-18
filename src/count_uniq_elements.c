#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#define WORK_DIR "./work_dir"
#define CHUNK_SIZE 25
#define LINE_SIZE 10
#define MAX_FILES_IN_WORK_DIR 200


uint32_t save_to_file(const int* arr, const uint64_t arr_size, const char* file_name);

typedef struct string_arr {
    char** elems;
    uint32_t size;
} string_arr;

void free_strings(const string_arr* string_arr) {
    if (string_arr == NULL) return;
    for (int i = 0; i < string_arr->size; ++i) {
        free(string_arr->elems[i]);
    }
}

uint64_t read_file(FILE* file, char* buf_line, int* result) {
    uint64_t idx = 0;

    while (fgets(buf_line, LINE_SIZE, file)) {
        result[idx] = atoi(buf_line);
        idx++;
    }

    return idx; // len of result arr
}

uint32_t merge_files(char* file_path1, char* file_path2, int* buf_a, int* buf_b,
                     int* result_buf, char* buf_line) {
    if (file_path1 == NULL || file_path2 == NULL) {
        perror("file path is null\n");
        return -1;
    }
    // read first file
    FILE* file1 = fopen(file_path1, "r");
    const uint64_t read_a = read_file(file1, buf_line, buf_a);
    fclose(file1);
    remove(file_path1);
    // read second file
    FILE* file2 = fopen(file_path2, "r");
    const uint64_t read_b = read_file(file2, buf_line, buf_b);
    fclose(file2);
    remove(file_path2);

    // merge two buffer in result buffer
    uint64_t idx_a = 0, idx_b = 0, result_idx = 0;
    while (idx_a < read_a && idx_b < read_b) {
        if (buf_a[idx_a] < buf_b[idx_b]) {
            result_buf[result_idx++] = buf_a[idx_a++];
        }
        else {
            result_buf[result_idx++] = buf_b[idx_b++];
        }
    }

    while (idx_a < read_a) {
        result_buf[result_idx++] = buf_a[idx_a++];
    }

    while (idx_b < read_b) {
        result_buf[result_idx++] = buf_b[idx_b++];
    }

    // save data from result buffer to a file
    char new_file_name[100];
    const int last_digit1 = atoi(&file_path1[strlen(file_path1) - 1]);
    const int last_digit2 = atoi(&file_path2[strlen(file_path2) - 1]);
    sprintf(new_file_name, "%s__%d_%d", file_path1, last_digit1, last_digit2);
    save_to_file(result_buf, result_idx, new_file_name);

    return 0;
}

int compare(const void* a, const void* b) {
    const int int_a = *((int *)a);
    const int int_b = *((int *)b);

    if (int_a == int_b) return 0;
    else if (int_a < int_b) return -1;
    else return 1;
}

uint32_t create_one_sorted_file() {
    DIR* d = opendir(WORK_DIR);
    if (d == NULL) {
        perror("cannot open work dir\n");
        return -1;
    }
    const uint32_t workdir_len = strlen(WORK_DIR);
    char* buf[MAX_FILES_IN_WORK_DIR];
    string_arr file_name_arr = {.elems = buf};

    int* buf_s1 = malloc(sizeof(int) * CHUNK_SIZE);
    if (buf_s1 == NULL) {
        perror("cannot allocate memory for buf_s1\n");
        return 2;
    }
    int* buf_s2 = malloc(sizeof(int) * CHUNK_SIZE);
    if (buf_s2 == NULL) {
        perror("cannot allocate memory for buf_s2\n");
        return 2;
    }

    struct dirent* dir;
    while (true) {
        uint32_t idx = 0;
        file_name_arr.size = 0;
        // get file names in word dir
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_name[0] != '.') {
                const int file_name_len = strlen(dir->d_name) + workdir_len + 2;
                // allocate file_names strings
                char* file_name = malloc(file_name_len * sizeof(char));
                sprintf(file_name, "%s/%s", WORK_DIR, dir->d_name);
                file_name[file_name_len - 1] = '\0';
                file_name_arr.elems[idx++] = file_name;
                file_name_arr.size++;
            }
        }

        if (file_name_arr.size == 0) break;

        // merge_files
        char buf_line[LINE_SIZE];
        int* result_buf = malloc(sizeof(int) * 2 * CHUNK_SIZE);
        uint32_t fwd_idx = 1, bwd_idx = 0;
        while (fwd_idx < file_name_arr.size) {
            if (merge_files(file_name_arr.elems[bwd_idx], file_name_arr.elems[fwd_idx],
                            buf_s1, buf_s2, result_buf, buf_line) != 0) {
                fprintf(stderr, "cannot merge files. [%s] [%s]\n",
                        file_name_arr.elems[bwd_idx],
                        file_name_arr.elems[fwd_idx]);
                free(result_buf);
                free_strings(&file_name_arr);
                free(buf_s1);
                free(buf_s2);
                return -1;
            }
            bwd_idx += 2;
            fwd_idx += 2;
        }

        // free strings in sring arr

        free_strings(&file_name_arr);
        free(buf_s1);
        free(buf_s2);
        free(result_buf);
    }

    closedir(d);

    return 0;
}


uint32_t save_to_file(const int* arr, const uint64_t arr_size, const char* file_name) {
    if (file_name == NULL) {
        perror("file_name is null\n");
        return -1;
    }
    FILE* new_chunk_file = fopen(file_name, "wa");

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

uint32_t save_to_file_with_prefix(const int* arr, const uint64_t arr_size, const uint32_t postfix) {
    char chunk_file_path[50];
    sprintf(chunk_file_path, "%s/file_%d", WORK_DIR, postfix);
    save_to_file(arr, arr_size, chunk_file_path);

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
    int* chunk_buf = malloc(sizeof(int) * CHUNK_SIZE);
    if (chunk_buf == NULL) {
        perror("cannot allocate memory for chunk_buf\n");
        return -1;
    }

    char buf_line[LINE_SIZE];
    memset(chunk_buf, 0, CHUNK_SIZE * sizeof(char));
    memset(buf_line, 0, LINE_SIZE * sizeof(char));

    // read a chunk of big file, sort it and store it to file system
    uint32_t chunk_idx = 0, file_count = 0;
    while (fgets(buf_line, LINE_SIZE, file)) {
        chunk_buf[chunk_idx] = atoi(buf_line);
        chunk_idx++;
        if (chunk_idx >= CHUNK_SIZE) {
            qsort(chunk_buf, CHUNK_SIZE, sizeof(int), compare);
            if (file_count > MAX_FILES_IN_WORK_DIR) {
                perror("too much files in work dir. increase chunk size\n");
                free(chunk_buf);
                return -1;
            }
            save_to_file_with_prefix(chunk_buf, CHUNK_SIZE, file_count++);
            chunk_idx = 0;
        }
    }

    if (chunk_idx != 0) {
        const uint32_t len = chunk_idx;
        qsort(chunk_buf, len, sizeof(int), compare);
        save_to_file_with_prefix(chunk_buf, len, file_count);
    }

    free(chunk_buf);

    // recursevly merge files
    const uint32_t res = create_one_sorted_file(WORK_DIR);
    if (res != 0) {
        return -1;
    }
    // count unique elements in big sorted files
    // TODO
    fclose(file);
    return 0;
}
