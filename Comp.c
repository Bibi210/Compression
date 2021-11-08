#include "./Lib/ima.h"
#include "./Lib/list.h"
#include <stdlib.h>

#include <time.h>
#define TRUE 0
#define FALSE -1

Image *image;
list_t Graphe;

typedef struct Color {
  GLubyte Red, Green, Blue;
} Color_t;

int color_equal(const Color_t a, const Color_t b) {
  return a.Blue == b.Blue && a.Green == b.Green && a.Red == b.Red;
}

Color_t get_pixel_color(int pixel) {
  Color_t output = {image->data[pixel], image->data[pixel + 1],
                    image->data[pixel + 2]};
  return output;
}

typedef struct Pixel_Node {
  int pixel;
  list_t nexts;
} Pixel_Node_t;

typedef struct Region { //* Composente Connexte
  Pixel_Node_t Start;
  list_t Bords;
  Color_t Region_Color;
} Region_t;

int is_in_Region(Pixel_Node_t *Start, int pixel) { //! To test
  if (Start->pixel == pixel)
    return TRUE;

  Pixel_Node_t *tmp;
  size_t i = 0;
  for (; i < Start->nexts.size; i++) {
    tmp = see_elem(&Start->nexts, i);
    if (is_in_Region(tmp, pixel) == TRUE) {
      return TRUE;
    }
  }
  return FALSE;
}

int pixel_in_image(int pixel) {
  if (pixel < 0 || pixel >= (image->sizeX * image->sizeY))
    return FALSE;
  return TRUE;
}

int is_in_Graphe(int pixel) {
  if (pixel_in_image(pixel) == FALSE)
    return FALSE;

  size_t i = 0;
  Region_t *tmp;
  for (; i < Graphe.size; i++) {
    tmp = see_elem(&Graphe, i);
    if (is_in_Region(&tmp->Start, pixel) == TRUE)
      return TRUE;
  }
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

Region_t get_new() {
  Pixel_Node_t Start;
  Region_t output;
  int start_pixel;
  srand(time(NULL));
  do {
    start_pixel = rand() % (image->sizeX * image->sizeY);
  } while (is_in_Graphe(start_pixel));
  Start.pixel = start_pixel;

  // Crée Le Graphe Depuis Start 
  //TODO Get_Next_Pixels

  output.Region_Color = get_pixel_color(start_pixel);
  output.Start = Start;
  return output;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage : Compress FileName\n");
    exit(0);
  }

  if (Init(argv[1]) == EXIT_FAILURE) {
    fprintf(stderr, "Image initialization Failed\n");
    exit(0);
  };
  Graphe = init_list(sizeof(Region_t), NULL, NULL);
}