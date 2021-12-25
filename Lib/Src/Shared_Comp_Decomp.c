#include "../Shared_Comp_Decomp.h"

int globl_delta;

void Display_Color(Color_t color) {
  printf("R : %d | G : %d | B : %d\n", color.Red, color.Green, color.Blue);
}

int is_color_equal(const Color_t a, const Color_t b) {
  int current_delta = 0;
  current_delta += abs(a.Blue - b.Blue);
  current_delta += abs(a.Red - b.Red);
  current_delta += abs(a.Red - b.Red);
  if (current_delta <= globl_delta)
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
  printf("%llu \n", to_print->bords.size);
  printf("\n");
  for (size_t i = 0; i < to_print->bords.size; i++) {
    printf("|%d", *(int *)see_elem(&to_print->bords, i));
  }
  printf("\n");
}

void set_delta(int delta) { globl_delta = delta; }
