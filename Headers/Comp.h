#include "../Lib/Shared_Comp_Decomp.h"

int Init(char *s);

// Write integers < 4096 * 4096
int write_unsigned_int(FILE *to_write_in, uint32_t to_reduce);

void Free_All();

int Writecolor(FILE *dest, Color_t color);
Color_t get_pixel_color(int pixel);

struct Pixel_Node {
  int num;
  Color_t color;
  uint32_t nbs;
  struct aretes *nexts;
};
typedef struct Pixel_Node Pixel_Node_t;
typedef Pixel_Node_t *Pt_Pixel;
void add_next(Pt_Pixel node, int next_pixel);
void set_suivants(Pt_Pixel px);
void Display_Pixel(Pixel_Node_t *to_print);

struct G_node {
  int vu;
  Pt_Pixel node;
};

typedef struct G_node G_node_t;
typedef struct G_node *legraphe;

struct aretes {
  Pt_Pixel pt;
  float poids;
  struct aretes *suiv;
};
typedef struct aretes aretes;
typedef struct aretes *ptarete;

void WriteRegion(FILE *file, Region_t *to_write);
void build_reg(Region_t *to_build, int current_px, list_t *stack);
void find_bords(Region_t *to_build, int start);