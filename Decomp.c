#include "Lib/Shared_Comp_Decomp.h"
#include <stdio.h>

Image *image;

int read_image_sizes(FILE *fp) {
  uint8_t *read_bytes = malloc(sizeof(uint8_t) * 3);
  uint32_t tmp;
  if (fread(read_bytes, sizeof(uint8_t), 3, fp) != 3) {
    free(read_bytes);
    perror("Failed to read image sizes.");
    return FALSE;
  }
  tmp = *(uint32_t *)read_bytes;
  free(read_bytes);

  image->sizeX = tmp >> 12;
  image->sizeY = (tmp << 20) >> 20;
  image->image_size = image->sizeX * image->sizeY;
  image->data = calloc(image->sizeX * image->sizeY, sizeof(GLubyte));
  return TRUE;
}

void set_pixel_color(Color_t color, int pixel) {
  image->data[(pixel * 3)] = color.Red;
  image->data[(pixel * 3) + 1] = color.Green;
  image->data[(pixel * 3) + 2] = color.Blue;
}

int read_unsigned_int(FILE *fp, uint32_t *dst) {
  uint8_t *read_bytes = malloc(sizeof(uint8_t) * 3);
  if (fread(read_bytes, sizeof(uint8_t), 3, fp) != 3) {
    free(read_bytes);
    perror("Failed to read unsigned int.");
    return FALSE;
  }
  *dst = *(uint32_t *)read_bytes;
  free(read_bytes);
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

int ReadGraph(FILE *fp, list_t *Graph) {
  Region_t tmp;
  int is_end = FALSE;
  while (!feof(fp)) {
    if ((is_end = ReadRegion(fp, &tmp)) == FALSE) {
      fprintf(stderr, "Failed to read ReadRegion");
      return FALSE;
    }
    if (is_end == 0)
      break;

    pushfront_elem(Graph, &tmp);
  }
  return TRUE;
}

int main(int argc, char **argv) {
  image = malloc(sizeof(Image));
  if (argc < 2) {
    fprintf(stderr, "Usage : Decomp FileName\n");
    exit(0);
  }
  FILE *to_decom = fopen(argv[1], "rb");
  if (to_decom == NULL) {
    perror("Unable to open the compressed file :");
    exit(-1);
  }
  read_image_sizes(to_decom);
  puts("Decomp Start :\n");
  printf("Image SizeX %u | Image SizeY %u \n", image->sizeX, image->sizeY);

  list_t Graph = init_list(sizeof(Region_t), NULL, free_reg);
  ReadGraph(to_decom, &Graph);

  imagesave_PPM("Output.ppm", image);
  free(image->data);
  free(image);
  free_list(&Graph);
  fclose(to_decom);
}