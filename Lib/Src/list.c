#include "../list.h"

int int_equal(void *a, void *b) { return *(int *)a == *(int *)b ? 0 : -1; }

list_t init_list(size_t data_size, int (*comp_func)(void *, void *),
                 void (*free_func)(void *)) {
  if (free_func == NULL)
    free_func = free;
  list_t output = {data_size, 0, NULL, free_func, comp_func};
  if (output.comp_func == NULL) {
    //! if two type have same size bug
    if (output.data_size == sizeof(int)) {
      output.comp_func = int_equal;
    }
  }
  return output;
}

void free_list(list_t *list) {
  Node_t *i, *temp;
  for (i = list->first; i;) {
    temp = i;
    i = i->next;
    free_node(temp, list->free_func);
  }
}

Node_t *create_node(void *data, size_t data_size) {
  Node_t *node = malloc(sizeof(Node_t));
  assert(node);

  node->data = malloc(data_size);
  assert(node->data);

  memcpy(node->data, data, data_size);
  assert(node->data);

  node->next = NULL;
  return node;
}

void pushfront_elem(list_t *list, void *data) {
  Node_t *tmp, *node;
  node = create_node(data, list->data_size);

  tmp = list->first;
  list->first = node;
  list->first->next = tmp;

  list->size++;
}

void append_elem(list_t *list, void *data) {
  //! Au dev de choisir de free data ou non
  Node_t *i, *node;
  node = create_node(data, list->data_size);

  if (!list->size) {
    list->first = node;
    list->size++;
    return;
  }
  for (i = list->first; i->next != NULL; i = i->next)
    ;
  i->next = node;

  list->size++;
  return;
}

void print_list(list_t *list, void (*print_func)(void *data)) {
  Node_t *i;
  printf("Liste of %llu elements:\n", list->size);
  for (i = list->first; i != NULL; i = i->next) {
    print_func(i->data);
    puts("");
  }
}

void remove_elem(list_t *list, llu index) {
  Node_t *i, *tmp;
  list->size--;
  if (!index) {
    tmp = list->first;
    list->first = list->first->next;
    free_node(tmp, list->free_func);
    return;
  }

  for (i = list->first; i->next != NULL && index; i = i->next, index--)
    tmp = i;
  tmp->next = i->next;
  free_node(i, list->free_func);
}

void insert_elem(list_t *list, void *data, llu index) {
  Node_t *i, *tmp, *node;
  if (index == 0) {
    pushfront_elem(list, data);
    return;
  }
  if (index >= list->size) {
    append_elem(list, data);
    return;
  }
  node = create_node(data, list->data_size);
  for (i = list->first; i->next != NULL && index; i = i->next, index--)
    tmp = i;

  tmp->next = node;
  node->next = i;
  list->size++;
}

void *see_elem(list_t *list, llu index) {
  Node_t *i;
  if (list->size == 0)
    return NULL;
  for (i = list->first; i->next != NULL && index; i = i->next, index--)
    ;
  return i->data; //! Retourne un pointer vers le node a vos risques et perils
}

void free_node(Node_t *node, void (*free_func)(void *)) {
  free_func(node->data);
  free(node);
}

llu get_smallest_index(list_t *list) {
  llu i, best_index = 0;
  void *tmp_elem, *rst;
  rst = list->first->data;
  for (i = 0; i < list->size; i++) {
    tmp_elem = see_elem(list, i);
    if (list->comp_func) {
      if (list->comp_func(tmp_elem, rst)) {
        rst = tmp_elem;
        best_index = i;
      }
    } else {
      fprintf(stderr, "Dev Need to specify comp_func");
      exit(EXIT_FAILURE);
    }
  }
  return best_index;
}

int some(list_t *list, int (*Prelicat)(void *)) {
  llu i;
  for (i = 0; i < list->size; i++) {
    if (Prelicat(see_elem(list, i)) == 0)
      return 0;
  }
  return -1;
}

int is_in(list_t *list, void *to_find) {
  llu i;
  for (i = 0; i < list->size; i++) {
    if (list->comp_func(see_elem(list, i), to_find) == 0)
      return 0;
  }
  return -1;
}

void foreach (list_t *list, void (*to_apply)(void *)) {
  for (size_t i = 0; i < list->size; i++) {
    to_apply(see_elem(list, i));
  }
}

int equals(list_t *list, list_t *other) {
  if (list->size != other->size || list->data_size != other->data_size)
    return -1;
  for (size_t i = 0; i < list->size; i++) {
    if (list->comp_func(see_elem(list, i), see_elem(other, i)) != 0)
      return -1;
  }
  return 0;
}