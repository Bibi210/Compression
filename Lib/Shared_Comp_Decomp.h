#include "../Lib/ima.h"
#include "../Lib/list.h"
#include <_stdio.h>
#include <_types/_uint8_t.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX 4096 * 4096

#define TRUE 1
#define FALSE -1

void set_delta(int new_delta);

typedef struct Color {
  GLubyte Red, Green, Blue;
} Color_t;
void Display_Color(Color_t color);
int is_color_equal(const Color_t a, const Color_t b);
int is_pixel_in_image(int pixel, uint32_t image_size);

typedef struct Region_t {
  Color_t color;
  list_t bords;
  uint32_t reg_size;
} Region_t;
void print_region(Region_t *print);
void free_reg(void *to_free);
const char *get_filename_ext(const char *filename);