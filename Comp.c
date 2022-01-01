#include "Headers/Comp.h"
#include <stdint.h>
#include <stdio.h>
Image *image;
legraphe Graphe;
Region_t **MergedRegions;

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
  if (write_unsigned_int(file, to_write->solo_pair_count) == FALSE) {
    fprintf(stderr, "Failed to write solo_pair_count to file");
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
    Graphe[current_px].reg_id = to_build;
    pushfront_elem(&to_build->bords, &current_px);
  }
  for (ptarete i = Graphe[current_px].node->nexts; i; i = i->suiv) {
    pushfront_elem(stack, &i->pt->num);
  }
}

unsigned int from_color_to_uint(Color_t to_convert) {
  uint8_t bytes[3];
  bytes[0] = to_convert.Red;
  bytes[1] = to_convert.Blue;
  bytes[2] = to_convert.Green;
  return *(uint32_t *)bytes;
}

void find_bords(Region_t *to_build, int start) {
  list_t stack = init_list(sizeof(int), NULL, NULL);
  uint32_t elem;
  Color_t tmp_color;
  unsigned long long R = 0, G = 0, B = 0;
  pushfront_elem(&stack, &start);
  for (size_t i = 0; stack.size; i++) {
    elem = *(int *)see_elem(&stack, 0);
    remove_elem(&stack, 0);
    if (Graphe[elem].vu == FALSE) {
      build_reg(to_build, elem, &stack);
      Graphe[elem].vu = TRUE;
      tmp_color = get_pixel_color(elem);
      to_build->reg_size++;
      R += tmp_color.Red;
      G += tmp_color.Green;
      B += tmp_color.Blue;
    }
  }
  tmp_color.Red = R / to_build->reg_size;
  tmp_color.Green = G / to_build->reg_size;
  tmp_color.Blue = B / to_build->reg_size;
  MergedRegions[from_color_to_uint(tmp_color)] = to_build;
  to_build->color = tmp_color;
  free_list(&stack);
}

void merge_regions(list_t *reg_ls) {
  Region_t *tmp, *color_reg;
  int bord_member;
  for (size_t i = 0; i < reg_ls->size; i++) {
    tmp = see_elem(reg_ls, i);
    color_reg = MergedRegions[from_color_to_uint(tmp->color)];
    if (color_reg != tmp) {
      color_reg->reg_size += tmp->reg_size;
      while (tmp->bords.size) {
        bord_member = *(int *)see_elem(&tmp->bords, 0);
        remove_elem(&tmp->bords, 0);
        Graphe[bord_member].reg_id = color_reg;
      }
      remove_elem(reg_ls, i);
    }
  }
}

void line_pairs(list_t *reg_ls) {
  Region_t *tmp = NULL;
  uint32_t prec_pixel;
  int is_pair_start = TRUE;
  for (uint32_t i = 0; i < image->image_size; i++) {
    if (Graphe[i].reg_id != NULL && Graphe[i].reg_id != tmp) {
      if (is_pair_start == TRUE) {
        tmp = Graphe[i].reg_id;
        prec_pixel = i;
        is_pair_start = FALSE;
      } else {
        i--;
        is_pair_start = TRUE;
        if (i == prec_pixel) {
          append_elem(&tmp->bords, &i);
          tmp->solo_pair_count++;
        } else {
          pushfront_elem(&tmp->bords, &i);
          pushfront_elem(&tmp->bords, &prec_pixel);
        }
      }
    }
  }
  if (is_pair_start == FALSE) {
    uint32_t end_tmp = image->image_size - 1;
    if (end_tmp == prec_pixel) {
      append_elem(&tmp->bords, &end_tmp);
      tmp->solo_pair_count++;
    } else {
      pushfront_elem(&tmp->bords, &end_tmp);
      pushfront_elem(&tmp->bords, &prec_pixel);
    }
  }

  int t = 0;
  for (size_t i = 0; i < reg_ls->size; i++) {
    tmp = see_elem(reg_ls, i);
    if (tmp->solo_pair_count > 0) {
      t++;
    }
  }
  printf("%f %% of Regions With Solo Pixels Pairs\n",
         (double)t / (double)reg_ls->size * 100);
}

double get_file_size(FILE *fp) {
  long size;
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return size;
}

int main(int argc, char **argv) {
  time_t t0, t1;
  char *filename = argv[1];
  char *comp_filename = calloc(strlen(filename) + 20, sizeof(uint8_t));
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

  t0 = time(NULL);
  Graphe = malloc(sizeof(struct G_node) * image->image_size);
  assert(Graphe);

  for (size_t i = 0; i < image->image_size; i++) {
    Graphe[i].vu = FALSE;
    Graphe[i].reg_id = NULL;
    Graphe[i].node = malloc(sizeof(Pixel_Node_t));
    assert(Graphe[i].node);
    Graphe[i].node->color = get_pixel_color(i);
    Graphe[i].node->num = i;
    Graphe[i].node->nbs = 0;
    Graphe[i].node->nexts = NULL;
  }
  for (size_t i = 0; i < image->image_size; i++) {
    set_suivants(Graphe[i].node);
  }
  list_t reg_ls = init_list(sizeof(Region_t), NULL, free_reg);
  MergedRegions = calloc(16777215, sizeof(Region_t *));
  puts("Region Bords Recherch Start");
  for (size_t i = 0; i < image->image_size; i++) {
    if (Graphe[i].vu == FALSE) {
      Region_t *in_build = malloc(sizeof(Region_t));
      in_build->reg_size = 0;
      in_build->solo_pair_count = 0;
      in_build->bords = init_list(sizeof(int), NULL, NULL);
      find_bords(in_build, i);
      pushfront_elem_no_cpy(&reg_ls, in_build);
    }
  }

  printf("%llu Regions to process Before Merge \n", reg_ls.size);

  merge_regions(&reg_ls); // Est-ce hors sujet ?
  for (size_t i = 0; i < reg_ls.size; i++) {
    Region_t *tmp = see_elem(&reg_ls, i);
    free_list(&tmp->bords);
  }
  printf("%llu Regions to process After Merge \n", reg_ls.size);

  puts("Region Bords Pairs Recherch Start");
  line_pairs(&reg_ls);
  puts("Region Bords Pairs Recherch Done");

  puts("Compressed File Write Start");
  FILE *fp = fopen(comp_filename, "wb+");
  free(comp_filename);

  if (fp == NULL) {
    perror("File Compression File Open Error : ");
    Free_All();
    exit(-1);
  }
  if (write_unsigned_int(fp, image->sizeX) == FALSE)
    exit(-1);
  if (write_unsigned_int(fp, image->sizeY) == FALSE)
    exit(-1);
  for (size_t i = 0; i < reg_ls.size; i++) {
    Region_t *tmp = see_elem(&reg_ls, i);
      WriteRegion(fp, tmp);
    
  }
  puts("Compressed File Write Done");
  t1 = time(NULL);
  printf("Temps de Passer a la Compression = %f secondes \n", difftime(t1, t0));

  FILE *tmp = fopen(argv[1], "rb");

  printf("Taux de Compression = %f %%\n",
         (1 - (get_file_size(fp) / get_file_size(tmp))) * 100);
  fclose(tmp);
  fclose(fp);
  free(MergedRegions);
  free_list(&reg_ls);
  Free_All();
}
