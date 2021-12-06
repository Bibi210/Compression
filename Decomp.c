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
  long long nb;
  long to_push;
  long test;
  srand(time(NULL));
  list_t test_A = init_list(sizeof(long), cmp_long, free);
  time_t t0 = time(NULL);
  long max = 100;
  for (nb = 0; nb < max; nb++) {
    to_push = nb;
    test = max - nb;
    pushfront_elem(&test_A, &to_push);
    printf("%llu\n", nb);
   // is_in(&test_A, &test);  //! Slow down this shit  : 
  }
  time_t t1 = time(NULL);
  printf("Durée = %f\n", difftime(t1, t0));

  free_list(&test_A);

  return EXIT_SUCCESS;
}

//TODO Fill Line by Line Bords A to Bords B