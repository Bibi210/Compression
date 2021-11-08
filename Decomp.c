#include "./Lib/ima.h"
#include "./Lib/list.h"
#include <stdlib.h>
#include <time.h>

int cmp_long(void *a, void *b) { return *(long *)a < *(long *)b; }
int i;
int some_tst(void *a) {
  if (*(long *)a == i)
    return 0;
  return 1;
}
void print_long(void *value) { printf("%ld \n", *(long *)value); }
void add_long(void *a) {
  long *tmp = (long *)a;
  (*tmp) += 10;
}

int main(int argc, char **argv) {
  int nb;
  long to_push;
  srand(time(NULL));
  list_t test_A = init_list(sizeof(long), cmp_long, free);
  for (nb = 0; nb < 100; nb++) {
    to_push = nb;
    append_elem(&test_A, &to_push); 
  }
  foreach (&test_A, add_long);
  printf("%d\n", some(&test_A, some_tst));
  free_list(&test_A);
  return EXIT_SUCCESS;
}