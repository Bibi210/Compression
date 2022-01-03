#include "../Shared_Comp_Decomp.h"

void Display_Color(Color_t color) {
  printf("R : %d | G : %d | B : %d\n", color.Red, color.Green, color.Blue);
}

Color_t get_pixel_color(int pixel, Image *image) {
  Color_t output = {image->data[(pixel * 3)], image->data[(pixel * 3) + 1],
                    image->data[(pixel * 3) + 2]};
  return output;
}

int is_color_equal(const Color_t a, const Color_t b, int delta) {
  int current_delta = 0;
  current_delta += abs(a.Blue - b.Blue);
  current_delta += abs(a.Red - b.Red);
  current_delta += abs(a.Red - b.Red);
  if (current_delta <= delta)
    return TRUE;
  return FALSE;
}

#include <math.h>
// Found Here : https://www.compuphase.com/cmetric.htm
//! Not Used But interesting Increase Compression Rate
int is_color_equal_more_technical(const Color_t a, const Color_t b, int delta) {
  uint32_t rmean = ((uint32_t)a.Red + (uint32_t)b.Red) / 2;
  uint32_t Red = (uint32_t)a.Red - (uint32_t)b.Red;
  uint32_t Green = (uint32_t)a.Green - (uint32_t)b.Green;
  uint32_t Blue = (uint32_t)a.Blue - (uint32_t)b.Blue;
  double current_delta =
      sqrt((((512 + rmean) * Red * Red) >> 8) + 4 * Green * Green +
           (((767 - rmean) * Blue * Blue) >> 8));
  if (current_delta <= delta)
    return TRUE;
  return FALSE;
}

int is_pixel_in_image(int pixel, uint32_t img_size) {
  if (pixel < 0 || pixel >= img_size) {
    return FALSE;
  }
  return TRUE;
}

void free_reg(void *to_free) {
  Region_t *to_free_reg = to_free;
  free_list(&to_free_reg->bords);
  free(to_free);
}

void print_region(Region_t *to_print) {
  puts("Region Display ");
  Display_Color(to_print->color);
  printf("Nombre de Pixel aux Bords: %llu \n", to_print->bords.size);
  printf("\n");
  for (size_t i = 0; i < to_print->bords.size; i++) {
    printf("|%d", *(int *)see_elem(&to_print->bords, i));
  }
  printf("\n");
}

const char *get_filename_ext(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename)
    return "";
  return dot + 1;
}

uint32_t from_color_to_uint(Color_t to_convert) {
  uint8_t bytes[3];
  bytes[0] = to_convert.Red;
  bytes[1] = to_convert.Blue;
  bytes[2] = to_convert.Green;
  return *(uint32_t *)bytes;
}
