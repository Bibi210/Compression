#ifndef Shared_Comp_Decomp_h

#include "../Lib/ima.h"
#include "../Lib/list.h"
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


#define TRUE 1
#define FALSE -1

// Color Struct
typedef struct Color {
  GLubyte Red, Green, Blue;
} Color_t;
uint32_t from_color_to_uint(Color_t to_convert);
// Debug Display
void Display_Color(Color_t color);

// Test if two colors are "equal"
int is_color_equal(const Color_t a, const Color_t b, int);

// Test if a pixel is in image
int is_pixel_in_image(int pixel, uint32_t image_size);

// Return a Color_t with the pixel color
Color_t get_pixel_color(int pixel, Image *);

// 1 Usage Compossante Connexte du Graph
// 2 Usage Regroupement de Compossante Connexte avec la même Couleur
typedef struct Region_t {
  Color_t color;
  list_t bords;
  int reg_id;
  uint32_t reg_size; // Nombre de pixel d'un region
  uint32_t solo_pair_count; // Nombre d'autopair de pixels
} Region_t;
// Debug Display
void print_region(Region_t *print);
void free_reg(void *to_free);
const char *get_filename_ext(const char *filename);

#define Shared_Comp_Decomp_h
#endif // !Shared_Comp_Decomp_h
