#include "src/hash_map.h"
#include <stdio.h>

int main() {
  char k1[] = {'h', 'e', 'l'};
  char *k2 = "hel";

  printf("result %lu\n", hashcode(k1));
  printf("result %lu\n", hashcode(k2));
  return 0;
}