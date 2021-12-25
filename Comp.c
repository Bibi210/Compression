#include "Headers/Comp.h"
Image *image;
legraphe Graphe;

Color_t get_pixel_color(int pixel) {
  Color_t output = {image->data[(pixel * 3)], image->data[(pixel * 3) + 1],
                    image->data[(pixel * 3) + 2]};
  return output;
}

void Display_Pixel(Pixel_Node_t *to_print) {
  printf("Pixel Index : %d\n", to_print->num);
  Display_Color(to_print->color);
  for (ptarete i = to_print->nexts; i != NULL; i = i->suiv) {
    printf("Succsesor index = %d |", i->pt->num);
  }
  printf("\n");
}

void add_next(Pt_Pixel node, int next_pixel) {
  ptarete new_succ = malloc(sizeof(aretes));
  new_succ->pt = Graphe[next_pixel].node;
  new_succ->suiv = node->nexts;
  node->nexts = new_succ;
  node->nbs++;
}

void set_suivants(Pt_Pixel px) {
  int suivants[4];
  suivants[0] = px->num + image->sizeX;
  suivants[1] = px->num - image->sizeX;
  suivants[2] = px->num + 1;
  suivants[3] = px->num - 1;

  for (size_t i = 0; i < 4; i++) {
    if (is_pixel_in_image(suivants[i], image->image_size) == TRUE &&
        is_color_equal(px->color, Graphe[suivants[i]].node->color) == TRUE) {
      add_next(px, suivants[i]);
    }
  }
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

void Free_All() {
  ptarete temp;
  for (size_t i = 0; i < image->image_size; i++) {
    for (ptarete t = Graphe[i].node->nexts; t;) {
      temp = t;
      t = t->suiv;
      free(temp);
    }
    free(Graphe[i].node);
  }
  free(image->data);
  free(Graphe);
  free(image);
}

int write_unsigned_int(FILE *to_write_in, uint32_t to_reduce) {
  if (fwrite(&to_reduce, sizeof(uint8_t), 3, to_write_in) != 3) {
    perror("Failed to an unsigned integer");
    return FALSE;
  }
  return TRUE;
}

int write_img_size(FILE *to_write) {
  write_unsigned_int(to_write, image->sizeX);
  write_unsigned_int(to_write, image->sizeY);
  return TRUE;
}

int Writecolor(FILE *dest, Color_t color) {
  if (fwrite(&color, sizeof(Color_t), 1, dest) != TRUE) {
    perror("Failed to write color");
    return FALSE;
  }
  return TRUE;
}

void WriteRegion(FILE *file, Region_t *to_write) {
  uint32_t current_px_index;
  if (Writecolor(file, to_write->color) == FALSE) {
    fprintf(stderr, "Failed to write region color to file");
    exit(EXIT_FAILURE);
  }
  if (write_unsigned_int(file, to_write->bords.size) == FALSE) {
    fprintf(stderr, "Failed to write region bords size to file");
    exit(EXIT_FAILURE);
  }
  for (size_t i = to_write->bords.size - 1; i != -1; i--) {
    current_px_index = *(int *)see_elem(&to_write->bords, i);
    if (write_unsigned_int(file, current_px_index) == FALSE) {
      fprintf(stderr, "Failed to write a pixel");
    }
  }
}

void build_reg(Region_t *to_build, int current_px, list_t *stack) {
  if (Graphe[current_px].node->nbs < 4) {
    pushfront_elem(&to_build->bords, &current_px);
  }
  for (ptarete i = Graphe[current_px].node->nexts; i; i = i->suiv) {
    pushfront_elem(stack, &i->pt->num);
  }
}

void find_bords(Region_t *to_build, int start) {
  list_t stack = init_list(sizeof(int), NULL, NULL);
  int elem;
  Color_t tmp_color;
  unsigned long long R = 0, G = 0, B = 0;
  pushfront_elem(&stack, &start);
  for (size_t i = 0; stack.size; i++) {
    elem = *(int *)see_elem(&stack, 0);
    remove_elem(&stack, 0);
    if (Graphe[elem].vu == FALSE) {
      build_reg(to_build, elem, &stack);
      Graphe[elem].vu = TRUE;
      to_build->reg_size++;
      tmp_color = get_pixel_color(elem);
      R += tmp_color.Red;
      G += tmp_color.Green;
      B += tmp_color.Blue;
    }
  }
  tmp_color.Red = R / to_build->reg_size;
  tmp_color.Green = G / to_build->reg_size;
  tmp_color.Blue = B / to_build->reg_size;
  to_build->color = tmp_color;
  free_list(&stack);
}

int main(int argc, char **argv) {
  char *filename = argv[1];
  char *comp_filename = malloc(strlen(filename) + 20);
  strncpy(comp_filename, filename, strcspn(filename, "."));
  strcat(comp_filename, ".Compressed");

  if (argc < 3) {
    fprintf(stderr, "Usage : Comp FileName Equal_Delta\n");
    exit(0);
  }
  if (Init(filename) == EXIT_FAILURE) {
    fprintf(stderr, "Image initialization Failed\n");
    exit(0);
  }
  set_delta(atoi(argv[2]));
  Graphe = malloc(sizeof(struct G_node) * image->image_size);
  assert(Graphe);

  for (size_t i = 0; i < image->image_size; i++) {
    Graphe[i].vu = FALSE;
    Graphe[i].node = malloc(sizeof(Pixel_Node_t));
    Graphe[i].node->color = get_pixel_color(i);
    Graphe[i].node->num = i;
    Graphe[i].node->nbs = 0;
    Graphe[i].node->nexts = malloc(sizeof(aretes));
    Graphe[i].node->nexts = NULL;
  }

  for (size_t i = 0; i < image->image_size; i++) {
    set_suivants(Graphe[i].node);
  }

  FILE *fp = fopen(comp_filename, "wb+");
  if (fp == NULL) {
    perror("File Compression File Open Error : ");
    Free_All();
    exit(-1);
  }
  write_img_size(fp);
  for (size_t i = 0; i < image->image_size; i++) {
    if (Graphe[i].vu == FALSE) {
      Region_t in_build;
      in_build.reg_size = 0;
      in_build.bords = init_list(sizeof(int), NULL, NULL);
      find_bords(&in_build, i);
      if (in_build.bords.size > 3) {
        WriteRegion(fp, &in_build);
      }
      free_list(&in_build.bords);
    }
  }

  fclose(fp);
  Free_All();
}
