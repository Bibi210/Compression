#ifndef List_h
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long llu;
typedef struct Node {
  void *data;        // Generic data
  struct Node *next; // Nexts
} Node_t;
// Free the node using the specified function
void free_node(Node_t *node, void (*free_func)(void *));

typedef struct list {
  size_t data_size;                 // Size of data
  llu size;                         // List Length
  Node_t *first;                    // First node
  void (*free_func)(void *);        // Data Free function
  int (*comp_func)(void *, void *); // Compair function for Sorts
} list_t;

//* Init List
list_t init_list(size_t data_size, int (*comp_func)(void *, void *),
                 void (*free_func)(void *));
list_t *init_list_ptr(size_t data_size, int (*comp_func)(void *, void *),
                      void (*free_func)(void *));
//* Init List

// Free List Using Specified Function
void free_list(list_t *list);

// Get Pointer to Elem
void *see_elem(list_t *list, llu index);

// Remove at
void remove_elem(list_t *list, llu index);
void remove_elem_no_free(list_t *list, llu index);

//* Data Adding Functions
void append_elem(list_t *list, void *data);
void pushfront_elem(list_t *list, void *data);
void pushfront_elem_no_cpy(list_t *list, void *data);

// Insert at
void insert_elem(list_t *list, void *data, llu index);
//* Data Adding Functions

//* Display List using the specified function
void print_list(list_t *list, void (*print_func)(void *data));

//* Requires Comp_Func
list_t insert_sort(list_t *list);
llu get_smallest_index(list_t *list);
llu get_insert_index(list_t *list, void *data);
int equals(list_t *list, list_t *other);
//* Requires Comp_Func

//* Fancy Stuffs
int is_in(list_t *list, void *to_find);
void foreach (list_t *list, void (*to_apply)(void *, void *),
              void *func_params);
int some(list_t *list, int (*Prelicat)(void *));
//* Fancy Stuffs

#define List_h
#endif // !List_h
