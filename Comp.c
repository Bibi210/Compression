#include "Lib/ima.h"
#include <assert.h>
#define MAX 4096 * 4096
Image *image;
int delta;

#define TRUE 1
#define FALSE 0

typedef struct Color {
  GLubyte Red, Green, Blue;
} Color_t;
void Display_Color(Color_t color) {
  printf("R : %d | G : %d | B : %d\n", color.Red, color.Green, color.Blue);
}

int is_color_equal(const Color_t a, const Color_t b) {
  int current_delta = 0;
  current_delta += abs(a.Blue - b.Blue);
  current_delta += abs(a.Red - b.Red);
  current_delta += abs(a.Red - b.Red);
  if (current_delta <= delta)
    return TRUE;

  return FALSE;
}
struct Pixel_Node {
  int num;
  Color_t color;
  int nbs;
  struct aretes *nexts;
};
typedef struct Pixel_Node Pixel_Node_t;
typedef Pixel_Node_t *Pt_Pixel;
int is_pixel_in_image(int pixel) {
  if (pixel < 0 || (unsigned)pixel >= image->image_size)
    return FALSE;
  return TRUE;
}

struct G_node {
  int vu;
  Pt_Pixel node;
};
typedef struct G_node G_node_t;
typedef struct G_node *legraphe;
legraphe Graphe;

struct aretes {
  Pt_Pixel pt;
  float poids;
  struct aretes *suiv;
};
typedef struct aretes aretes;
typedef struct aretes *ptarete;

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
    if (is_pixel_in_image(suivants[i]) &&
        is_color_equal(px->color, Graphe[suivants[i]].node->color)) {
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



int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Usage : Comp FileName Equal_Delta\n");
    exit(0);
  }
  if (Init(argv[1]) == EXIT_FAILURE) {
    fprintf(stderr, "Image initialization Failed\n");
    exit(0);
  }
  delta = atoi(argv[2]);

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

  for (size_t i = 0; i < image->image_size; i++) {
    Display_Pixel(Graphe[i].node);
  }

  Free_All();
}