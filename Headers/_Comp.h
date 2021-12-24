#include "../Lib/ima.h"
#include "../Lib/list.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TRUE 0
#define FALSE -1

int Init(char *s);
void *is_in_Graphe(int pixel);

typedef struct Color {
  int Red, Green, Blue;
} Color_t;
Color_t get_pixel_color(int pixel);
void Display_Color(Color_t color);
int is_color_equal(const Color_t a, const Color_t b);

typedef struct Pixel_Node {
  int pixel;
  list_t *nexts;
} Pixel_Node_t;
void Display_Pixel(Pixel_Node_t *to_print);
int is_pixel_in_image(int pixel);
void set_pixel_succesors(Pixel_Node_t *current, Color_t Region_Color,
                         list_t *Bords);
void set_pixel_color(int pixel, Color_t new_color);

typedef struct Region { //* Composente Connexte
  Pixel_Node_t *Start;
  list_t* Bords;
  Color_t Region_Color;
  llu nb_pixel;
} Region_t;
Region_t* init_region(int start_pixel);
void print_region(Region_t reg);

