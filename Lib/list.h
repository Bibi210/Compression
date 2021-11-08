#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long llu;
typedef struct Node {
  void *data;
  struct Node *next;
} Node_t;

typedef struct list {
  size_t data_size;
  llu size;
  Node_t *first;
  void (*free_func)(void *);
  int (*comp_func)(void *, void *);
} list_t;


list_t init_list(size_t data_size, int (*comp_func)(void *, void *),
                 void (*free_func)(void *));
void *see_elem(list_t *list, llu index); //! Untested
void insert_elem(list_t *list, void *data, llu index);
void remove_elem(list_t *list, llu index);
void append_elem(list_t *list, void *data);
void pushfront_elem(list_t *list, void *data);
void print_list(list_t *list, void (*print_func)(void *data));
void free_list(list_t *list);
void free_node(Node_t *node, void (*free_func)(void *));
llu get_smallest_index(list_t *list);