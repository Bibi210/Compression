#include "./Lib/ima.h"
#include "./Lib/list.h"
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#define TRUE 0
#define FALSE -1

Image *image;
// Image Size = 786432
list_t Graphe;
typedef struct Pixel_Node {
  int pixel;
  list_t *nexts;
} Pixel_Node_t;

typedef struct Color {
  GLubyte Red, Green, Blue;
} Color_t;

Color_t get_pixel_color(int pixel) {
  Color_t output = {image->data[(pixel * 3)], image->data[(pixel * 3) + 1],
                    image->data[(pixel * 3) + 2]};
  return output;
}
void Display_Color(Color_t color) {
  printf("R : %d | G : %d | B : %d\n", color.Red, color.Green, color.Blue);
}
void Display_Pixel(Pixel_Node_t *to_print) {
  if (to_print != NULL) {
    printf("Pixel Index : %d\n", to_print->pixel);
    get_pixel_color(to_print->pixel);
  }
}
typedef struct Region { //* Composente Connexte
  Pixel_Node_t *Start;
  list_t Bords;
  Color_t Region_Color;
} Region_t;

Pixel_Node_t **Seen;

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
  if (pixel < 0 || (unsigned)pixel >= (image->sizeX * image->sizeY))
    return FALSE;
  return TRUE;
}

int color_equal(
    const Color_t a,
    const Color_t b) { // TODO Peux être adapté mais il y'aura de la perte
  if (a.Blue == b.Blue && a.Green == b.Green && a.Red == b.Red)
    return TRUE;
  return FALSE;
}

void print_region(Region_t reg) {
  printf("Bords Members :\n");
  for (size_t i = 0; i < reg.Bords.size; i++) {
    Pixel_Node_t *pixel = see_elem(&reg.Bords, i);
    printf("%d\n", pixel->pixel);
  }
}

void *is_in_Graphe(int pixel) {
  if (pixel_in_image(pixel) == FALSE)
    return NULL;
  if (Seen[pixel] != NULL) {
    return Seen[pixel];
  } else
    return NULL;
}

void get_nexts(Pixel_Node_t *current, Color_t Region_Color, list_t *Bords,
               list_t *Stack) {
  if (Seen[current->pixel] == NULL) {
    list_t *successors = malloc(
        sizeof(list_t)); // TODO Ne pas malloc si pas de successors trouver
    *successors = init_list(sizeof(Pixel_Node_t *), NULL, NULL);

    Seen[current->pixel] = current;

    Pixel_Node_t to_add;
    int up, down, left, right;
    up = current->pixel + image->sizeX;
    down = current->pixel - image->sizeX;
    right = current->pixel + 1;
    left = current->pixel - 1;

    if (pixel_in_image(up) == TRUE &&
        color_equal(Region_Color, get_pixel_color(up)) == TRUE) {
      if (is_in_Graphe(up) == NULL) {
        to_add.pixel = up;
        pushfront_elem(successors, &to_add);
      } else {
        pushfront_elem_no_cpy(successors, is_in_Graphe(up));
      }
    }

    if (pixel_in_image(down) == TRUE &&
        color_equal(Region_Color, get_pixel_color(down)) == TRUE) {
      if (is_in_Graphe(down) == NULL) {
        to_add.pixel = down;
        pushfront_elem(successors, &to_add);
      } else {
        pushfront_elem_no_cpy(successors, is_in_Graphe(down));
      }
    }

    if (pixel_in_image(left) == TRUE &&
        color_equal(Region_Color, get_pixel_color(left)) == TRUE) {
      if (is_in_Graphe(left) == NULL) {
        to_add.pixel = left;
        pushfront_elem(successors, &to_add);
      } else {
        pushfront_elem_no_cpy(successors, is_in_Graphe(left));
      }
    }

    if (pixel_in_image(right) == TRUE &&
        color_equal(Region_Color, get_pixel_color(right)) == TRUE) {
      if (is_in_Graphe(right) == NULL) {
        to_add.pixel = right;
        pushfront_elem(successors, &to_add);
      } else {
        pushfront_elem_no_cpy(successors, is_in_Graphe(right));
      }
    }

    for (size_t i = 0; i < successors->size && successors != NULL; i++) {
      if (is_in_Graphe(((Pixel_Node_t *)see_elem(successors, i))->pixel) ==
          NULL) {
        pushfront_elem_no_cpy(Stack, see_elem(successors, i));
      }
    }

    current->nexts = successors;
    if (current->nexts->size < 4)
      pushfront_elem(Bords, &current->pixel);
  }
  return;
}

Region_t get_new(int start_pixel) {
  Pixel_Node_t *Start = malloc(sizeof(Pixel_Node_t));
  Region_t output;

  srand(time(NULL));
  Start->pixel = start_pixel;
  output.Region_Color = get_pixel_color(start_pixel);
  output.Bords = init_list(sizeof(int), NULL, NULL);
  output.Start = Start;

  list_t Recursion_Stack = init_list(sizeof(Pixel_Node_t *), NULL, NULL);
  pushfront_elem(&Recursion_Stack, output.Start);
  while (Recursion_Stack.size) {
    Pixel_Node_t *to_proccess = see_elem(&Recursion_Stack, 0);
    remove_elem_no_free(&Recursion_Stack, 0);
    get_nexts(to_proccess, output.Region_Color, &output.Bords,
              &Recursion_Stack);
  }
  free_list(&Recursion_Stack);

  return output;
}

int main(int argc, char **argv) {
  // TODO les frees
  if (argc < 2) {
    fprintf(stderr, "Usage : Compress FileName\n");
    exit(0);
  }

  if (Init(argv[1]) == EXIT_FAILURE) {
    fprintf(stderr, "Image initialization Failed\n");
    exit(0);
  };

  Seen = malloc(sizeof(Pixel_Node_t *) * (image->sizeX * image->sizeY));
  for (size_t i = 0; i < image->sizeX * image->sizeY; i++) {
    Seen[i] = NULL;
  }

  Graphe = init_list(sizeof(Region_t), NULL, NULL);
  int start_pixel = 0;
  int new_img_size = 0;
  for (; (unsigned)start_pixel < (image->sizeX * image->sizeY); start_pixel++) {
    if (is_in_Graphe(start_pixel) == NULL) { 
      Region_t in_build = get_new(start_pixel);
      pushfront_elem(&Graphe, &in_build);
      new_img_size += in_build.Bords.size;
    }
  }
  printf("Image Base Pixels = %lu\n", image->sizeX * image->sizeY);
  printf("Image Stocked Pixels = %d\n", new_img_size);
  printf("Gain = %lu\n", (image->sizeX * image->sizeY) - new_img_size);
  
}