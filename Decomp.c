#include "Headers/Decomp.h"

Image *image;

void set_pixel_color(Color_t color, int pixel) {
  image->data[(pixel * 3)] = color.Red;
  image->data[(pixel * 3) + 1] = color.Green;
  image->data[(pixel * 3) + 2] = color.Blue;
}

int read_unsigned_int(FILE *fp, uint32_t *dst) {
  uint8_t *read_bytes = calloc(3, sizeof(uint8_t));
  if (fread(read_bytes, sizeof(uint8_t), 3, fp) != 3) {
    free(read_bytes);
    perror("Failed to read unsigned int.");
    return FALSE;
  }
  *dst = *(uint32_t *)read_bytes;
  free(read_bytes);
  return TRUE;
}

int read_image_sizes(FILE *fp) {
  uint32_t tmp;
  if (read_unsigned_int(fp, &tmp) == FALSE) {
    fprintf(stderr, "Failed to read image sizeX");
    exit(-1);
  }
  image->sizeX = tmp;
  if (read_unsigned_int(fp, &tmp) == FALSE) {
    fprintf(stderr, "Failed to read image sizeY");
    exit(-1);
  }
  image->sizeY = tmp;
  image->image_size = image->sizeX * image->sizeY;
  image->data = calloc(image->image_size * 3, sizeof(GLubyte));

  return TRUE;
}

int ReadRegion(FILE *fp, Region_t *dst) {
  uint32_t nb_bords, tmp;
  int read_bytes;
  if ((read_bytes = fread(&dst->color, sizeof(Color_t), 1, fp)) != TRUE) {
    return read_bytes;
  }
  if (read_unsigned_int(fp, &nb_bords) == FALSE) {
    fprintf(stderr, "Failed to read region bords");
    return FALSE;
  }

  if (read_unsigned_int(fp, &dst->solo_pair_count) == FALSE) {
    fprintf(stderr, "Failed to read region bords");
    return FALSE;
  }

  dst->bords = init_list(sizeof(int), NULL, NULL);
  for (size_t i = 0; i < nb_bords; i++) {
    if (read_unsigned_int(fp, &tmp) == FALSE) {
      fprintf(stderr, "Failed to read bords member");
      return FALSE;
    }
    set_pixel_color(dst->color, tmp);
    pushfront_elem(&dst->bords, &tmp);
  }
  return TRUE;
}

int ReadGraph(FILE *fp) {
  Region_t tmp;
  int is_end = FALSE;
  size_t i;
  read_image_sizes(fp);
  printf(" Size X : %d | Size Y : %d\n", image->sizeX, image->sizeY);

  uint32_t pair_begin, pair_end;
  while (!feof(fp)) {
    if ((is_end = ReadRegion(fp, &tmp)) == FALSE) {
      fprintf(stderr, "Failed to read ReadRegion");
      return FALSE;
    }
    if (is_end == 0)
      return TRUE;
    for (i = 0; i < tmp.bords.size - tmp.solo_pair_count; i += 2) {
      pair_begin = *(uint32_t *)see_elem(&tmp.bords, i);
      pair_end = *(uint32_t *)see_elem(&tmp.bords, i + 1);
      for (size_t t = pair_begin; t <= pair_end; t++) {
        set_pixel_color(tmp.color, t);
      }
    }
    for (size_t t = 0; t < tmp.solo_pair_count; i++, t++) {
      set_pixel_color(tmp.color, *(uint32_t *)see_elem(&tmp.bords, i));
    }
    free_list(&tmp.bords);
  }

  return TRUE;
}

void Comparaison_Img(Image *A, Image *B) {
  Color_t A_current_pixel;
  Color_t B_current_pixel;
  double is_equal = 0;
  if (A->image_size != B->image_size) {
    fprintf(stderr, "The Two Image dont have the same sizes\n");
  } else {
    for (size_t i = 0; i < A->image_size; i++) {
      A_current_pixel = get_pixel_color(i, A);
      B_current_pixel = get_pixel_color(i, B);
      if (is_color_equal(A_current_pixel, B_current_pixel, 0) == FALSE)
        is_equal++;
    }
  }
  printf(" Taux de Pixels Differents %f %%\n",
         (100 * is_equal) / (double)B->image_size);
}

int main(int argc, char **argv) {
  clock_t t0, t1;
  char *filename = argv[1];
  char *origin_filename = calloc(strlen(filename) + 4, sizeof(uint8_t));
  strncpy(origin_filename, filename, strcspn(filename, "."));
  strcat(origin_filename, ".ppm");

  image = malloc(sizeof(Image));
  if (argc < 2 || strcmp("Compressed", get_filename_ext(argv[1]))) {
    fprintf(stderr, "Usage : Decomp FileName.Compressed\n");
    exit(0);
  }
  FILE *to_decomp = fopen(argv[1], "rb");
  if (to_decomp == NULL) {
    perror("Unable to open the compressed file :");
    exit(-1);
  }
  t0 = clock();
  printf("\nDebut Decompression de %s : \n", filename);
  ReadGraph(to_decomp);
  imagesave_PPM("Demo_Output.ppm", image);
  t1 = clock();
  printf(" Temps de Decompression = %f ms \n",
         (double)(t1 - t0) * 1000 / CLOCKS_PER_SEC);

  Image before;
  ImageLoad_PPM(origin_filename, &before);
  Comparaison_Img(&before, image);

  free(image->data);
  free(image);
  fclose(to_decomp);
}