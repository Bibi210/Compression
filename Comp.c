#include "Headers/Comp.h"
#include <stdlib.h>

Image *image;
list_t Graphe;
Pixel_Node_t **Seen;
list_t Stack;
int delta;

int is_color_equal(const Color_t a, const Color_t b) {
  int current_delta = 0;
  current_delta += abs(a.Blue - b.Blue);
  current_delta += abs(a.Red - b.Red);
  current_delta += abs(a.Red - b.Red);
  if (current_delta <= delta)
    return TRUE;

  return FALSE;
}

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

void *is_in_Graphe(int pixel) {
  if (is_pixel_in_image(pixel) == FALSE)
    return NULL;
  if (Seen[pixel] != NULL) {
    return Seen[pixel];
  } else
    return NULL;
}

Region_t *init_region(int start_pixel) {
  Pixel_Node_t *Start = malloc(sizeof(Pixel_Node_t)), *to_proccess;
  Region_t *output = malloc(sizeof(Region_t));

  Start->pixel = start_pixel;
  output->Region_Color = get_pixel_color(start_pixel);
  output->Bords = init_list_ptr(sizeof(int), NULL, NULL);
  output->Start = Start;

  pushfront_elem_no_cpy(&Stack, output->Start);
  while (Stack.size) {
    to_proccess = see_elem(&Stack, 0);
    if (Seen[to_proccess->pixel] == NULL) {
      output->nb_pixel++;
    }
    remove_elem_no_free(&Stack, 0);
    set_pixel_succesors(to_proccess, output->Region_Color, output->Bords);
  }
  free_list(&Stack);

  return output;
}

void set_pixel_succesors(Pixel_Node_t *current, Color_t Region_Color,
                         list_t *Bords) {
  if (!is_in_Graphe(current->pixel)) {
    Seen[current->pixel] = current;
    current->nexts = init_list_ptr(sizeof(Pixel_Node_t *), NULL, NULL);
    Pixel_Node_t to_add;
    int suivants[4];

    suivants[0] = current->pixel + image->sizeX;
    suivants[1] = current->pixel - image->sizeX;
    suivants[2] = current->pixel + 1;
    suivants[3] = current->pixel - 1;

    for (size_t i = 0; i < 4; i++) {
      if (is_pixel_in_image(suivants[i]) == TRUE &&
          is_color_equal(Region_Color, get_pixel_color(suivants[i])) == TRUE) {
        if (is_in_Graphe(suivants[i]) == NULL) {
          to_add.pixel = suivants[i];
          pushfront_elem(current->nexts, &to_add);
        } else {
          pushfront_elem_no_cpy(current->nexts, is_in_Graphe(suivants[i]));
        }
      }
    }

    for (size_t i = 0; i < current->nexts->size && current->nexts != NULL;
         i++) {
      if (!is_in_Graphe(((Pixel_Node_t *)see_elem(current->nexts, i))->pixel))
        pushfront_elem_no_cpy(&Stack, see_elem(current->nexts, i));
    }

    //Bords Easy Reacherche
     if (current->nexts->size < 4) {
      pushfront_elem_no_cpy(Bords, &current->pixel);
    } 
  }
}

// Basic and Utils Functions
void print_region(Region_t reg) {
  printf("Bords Members :\n");
  for (size_t i = 0; i < reg.Bords->size; i++) {
    Pixel_Node_t *pixel = see_elem(reg.Bords, i);
    printf("%d\n", pixel->pixel);
  }
}
int is_pixel_in_image(int pixel) {
  if (pixel < 0 || (unsigned)pixel >= image->image_size)
    return FALSE;
  return TRUE;
}
Color_t get_pixel_color(int pixel) {
  Color_t output = {image->data[(pixel * 3)], image->data[(pixel * 3) + 1],
                    image->data[(pixel * 3) + 2]};
  return output;
}
void set_pixel_color(int pixel, Color_t new_color) {
  image->data[(pixel * 3)] = new_color.Red;
  image->data[(pixel * 3) + 1] = new_color.Green;
  image->data[(pixel * 3) + 2] = new_color.Blue;
}
void Display_Color(Color_t color) {
  printf("R : %d | G : %d | B : %d\n", color.Red, color.Green, color.Blue);
}
void Display_Pixel(Pixel_Node_t *to_print) {
  printf("Pixel Index : %d\n", to_print->pixel);
  if (to_print->nexts == NULL) {
    puts("No next");
  } else {
    for (int i = 0; i < to_print->nexts->size; i++) {
      printf("Child %d Index : %d\n", i,
             ((Pixel_Node_t *)see_elem(to_print->nexts, i))->pixel);
    }
  }
}
// Basic and Utils Functions

void get_region_bords(Region_t *to_proccess) {
  pushfront_elem_no_cpy(&Stack, to_proccess->Start);

  while (Stack.size) {
    Pixel_Node_t *current_node = see_elem(&Stack, 0);
    // Display_Pixel(current_node);

    remove_elem_no_free(&Stack, 0);

    if (!is_in_Graphe(current_node->pixel)) {
      Seen[current_node->pixel] = current_node;

      /*  //! Bug 1
       if (current_node->nexts->size < 4)
         pushfront_elem(to_proccess->Bords, &current_node->pixel);
      */ //! Bug 1

      /* //! Bug 2
      for (size_t i = 0; i < current_node->nexts->size; i++) {
        if (!is_in_Graphe(
                ((Pixel_Node_t *)(see_elem(current_node->nexts, i)))->pixel)) {
          pushfront_elem_no_cpy(&Stack, see_elem(current_node->nexts, i));
        }
      }
      */ //! Bug 2
    }
  }
}
int main(int argc, char **argv) {
  // TODO les frees
  if (argc < 3) {
    fprintf(stderr, "Usage : Comp FileName Equal_Delta\n");
    exit(0);
  }

  if (Init(argv[1]) == EXIT_FAILURE) {
    fprintf(stderr, "Image initialization Failed\n");
    exit(0);
  };
  // Globals Initiatilisation
  delta = atoi(argv[2]);
  Seen = malloc(sizeof(Pixel_Node_t *) * image->image_size);
  Stack = init_list(sizeof(Pixel_Node_t *), NULL, NULL);
  for (size_t i = 0; i < image->image_size; i++) {
    Seen[i] = NULL;
  }
  Graphe = init_list(sizeof(Region_t *), NULL, NULL);
  // Globals Initiatilisation

  // Graph Creation
  int start_pixel = 0;
  int new_img_size = 0;
  for (; (unsigned)start_pixel < image->image_size; start_pixel++) {
    if (!is_in_Graphe(start_pixel)) {
      Region_t *in_build = init_region(start_pixel);
      pushfront_elem(&Graphe, in_build);
      new_img_size += in_build->Bords->size;
      /* for (size_t i = 0; i < in_build->Bords->size; i++) {
          Pixel_Node_t *tmp = see_elem(in_build->Bords, i);
          Color_t black = {0, 255, 0};
          set_pixel_color(tmp->pixel, black);
        }  */
    }
  }
  // Graph Creation

  // Bords Recherch
  /* for (size_t i = 0; i < image->image_size; i++)
    Seen[i] = NULL;

  for (size_t i = 0; i < Graphe.size; i++) {
    Region_t *in_build = (Region_t *)see_elem(&Graphe, i);
    get_region_bords(in_build);
  } */
  // Bords Recherch

  printf("Image Base Pixels = %lu\n", image->image_size);
  printf("Image Stocked Pixels = %d\n", new_img_size);
  printf("Gain = %lu\n", image->image_size - new_img_size);
  imagesave_PPM("demo_comp.ppm", image);
}