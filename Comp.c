#include "./Lib/ima.h"
#include "./Lib/list.h"
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#define TRUE 0
#define FALSE -1

Image *image;
list_t Graphe;

int Init(char *s) {
  image = (Image *)malloc(sizeof(Image));
  if (image == NULL) {
    fprintf(stderr, "Out of memory\n");
    return EXIT_FAILURE;
  }
  if (ImageLoad_PPM(s, image) == FALSE)
    return EXIT_FAILURE;
  printf("tailles %d %d\n", (int)image->sizeX, (int)image->sizeY);
  return EXIT_SUCCESS;
}

int pixel_in_image(int pixel) {
  if (pixel < 0 || pixel >= (image->sizeX * image->sizeY))
    return FALSE;
  return TRUE;
}

typedef struct Color {
  GLubyte Red, Green, Blue;
} Color_t;

int color_equal(
    const Color_t a,
    const Color_t b) { // TODO Peux être adapté mais il y'aura de la perte
  if (a.Blue == b.Blue && a.Green == b.Green && a.Red == b.Red)
    return TRUE;
  return FALSE;
}

Color_t get_pixel_color(int pixel) {
  Color_t output = {image->data[(pixel * 3)], image->data[(pixel * 3) + 1],
                    image->data[(pixel * 3) + 2]};
  return output;
}

typedef struct Pixel_Node {
  int pixel;
  list_t *nexts;
} Pixel_Node_t;

typedef struct Region { //* Composente Connexte
  Pixel_Node_t *Start;
  list_t Bords;
  Color_t Region_Color;
} Region_t;

int is_in_Region(Pixel_Node_t *Start, int pixel) { //! To test
  if (Start == NULL) {
    return FALSE;
  }

  if (Start->pixel == pixel) {
    return TRUE;
  }

  if (Start->nexts == NULL || Start->nexts->size == 0) {
    return FALSE;
  }

  Pixel_Node_t *tmp;
  size_t i = 0;
  for (; i < Start->nexts->size; i++) {
    tmp = see_elem(Start->nexts, i);
    if (is_in_Region(tmp, pixel) == TRUE)
      return TRUE;
  }
  return FALSE;
}

int is_in_Graphe(int pixel) {
  if (pixel_in_image(pixel) == FALSE)
    return FALSE;

  size_t i = 0;
  Region_t *tmp;
  for (; i < Graphe.size; i++) {
    tmp = see_elem(&Graphe, i);
    if (is_in_Region(tmp->Start, pixel) == TRUE)
      return TRUE;
  }
  return FALSE;
}

void get_nexts(Pixel_Node_t *current, Color_t Region_Color, list_t *Done) {
  list_t successors = init_list(sizeof(Pixel_Node_t *), NULL, NULL);
  Pixel_Node_t to_add;
  int up, down, left, right;
  up = current->pixel + image->sizeX;
  down = current->pixel - image->sizeX;
  right = current->pixel + 1;
  left = current->pixel - 1;

  if (pixel_in_image(up) == TRUE &&
      color_equal(Region_Color, get_pixel_color(up)) == TRUE) {
    to_add.pixel = up;
    pushfront_elem(&successors, &to_add);
  }
  if (pixel_in_image(down) == TRUE &&
      color_equal(Region_Color, get_pixel_color(down)) == TRUE) {
    to_add.pixel = down;
    pushfront_elem(&successors, &to_add);
  }
  if (pixel_in_image(left) == TRUE &&
      color_equal(Region_Color, get_pixel_color(left)) == TRUE) {
    to_add.pixel = left;
    pushfront_elem(&successors, &to_add);
  }
  if (pixel_in_image(right) == TRUE &&
      color_equal(Region_Color, get_pixel_color(right)) == TRUE) {
    to_add.pixel = right;
    pushfront_elem(&successors, &to_add);
  }

  Pixel_Node_t *tmp;
  for (size_t i = 0; i < successors.size; i++) {
    tmp = see_elem(&successors, i);
    if (is_in(Done, &tmp->pixel) == FALSE) {
      printf("Yolo");
      pushfront_elem(Done, &tmp->pixel);
      get_nexts(tmp, Region_Color, Done);
    }
  }
  current->nexts = &successors;

  return;
}

Region_t get_new() {
  list_t Done = init_list(sizeof(int), NULL, NULL);
  Pixel_Node_t Start;
  Region_t output;
  int start_pixel;
  long long size;
  srand(time(NULL));
  do { // Change Prendre premier pas in Graphe
    do {
      start_pixel = rand() % (image->sizeX * image->sizeY);
    } while (!is_in_Graphe(start_pixel));
    Start.pixel = start_pixel;
    output.Region_Color = get_pixel_color(start_pixel);
    output.Bords = init_list(sizeof(int), NULL, NULL);
    output.Start = &Start;

    get_nexts(output.Start, output.Region_Color, &Done); 
    //TODO (Upgrade) Si index déja existant prendre le pointeur vers cette index déja existant
    printf("successors = %llu\n", size = output.Start->nexts->size);

    output.Start = &Start;

  } while (size != 4);
  // TODO Durant le Parcours ajouter dans Bords les noeud avec moins de 4
  // connections

  return output;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage : Compress FileName\n");
    exit(0);
  }

  if (Init(argv[1]) == EXIT_FAILURE) {
    fprintf(stderr, "Image initialization Failed\n");
    exit(0);
  };
  Graphe = init_list(sizeof(Region_t), NULL, NULL);
  Region_t in_build = get_new();
  pushfront_elem(&Graphe, &in_build);
}