#include <stdio.h>
#include <unistd.h>
#include "mymalloc.h"
//include your code

//replace malloc here with the appropriate version of mymalloc
#define MALLOC my_bestfit_malloc
//replace free here with the appropriate version of myfree
#define FREE my_free

int main() {
  char *a, *b, *c, *d;
  printf("sbrk start: %d\n", (int)sbrk(0));
  a = MALLOC(14);
  printf("sbrk a: %d\n", (int)sbrk(0));
  b = MALLOC(14);
  printf("sbrk b: %d\n", (int)sbrk(0));
  c = MALLOC(14);
  printf("sbrk c: %d\n", (int)sbrk(0));
  d = MALLOC(14);
  printf("sbrk d: %d\n", (int)sbrk(0));
  printf("Free'ing C\n");
  FREE(d);
  printf("sbrk c free: %d\n", (int)sbrk(0));
  printf("Free'ing B\n");
  FREE(c);
  printf("sbrk b free: %d\n", (int)sbrk(0));
  printf("Free'ing D\n");
  FREE(b);
  printf("sbrk d free: %d\n", (int)sbrk(0));
  printf("Free'ing A\n");
  FREE(a);
  printf("sbrk a free: %d\n", (int)sbrk(0));
  a = MALLOC(14);
  printf("sbrk a: %d\n", (int)sbrk(0));
  return 0;
}
