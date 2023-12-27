#include "src/heap_hash_map.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  hash_map* hm = new_hash_map();   
  if (hm == NULL) {
    return 1;
  }
  char *k = "hello";
  char *res = malloc(6); 
  if (res == NULL) {
    return 2;
  }

  res = "world";

  uint32_t r = put_hash_map(hm, k, res);
  if (r != 0) {
    perror("cannot put hashmap\n");
    return 1;
  }

  //char* value = get_hash_map(hm, k);

  //printf("val %s\n", value);
  return 0;
}