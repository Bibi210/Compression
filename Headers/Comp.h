#ifndef Comp_h
#include "../Lib/Shared_Comp_Decomp.h"

int Init(char *s);

// Free Stuff
void Free_All();

//* Write Stuff

int Writecolor(FILE *dest, Color_t color);

// Write integers < 4096 * 4096 (On 3 bytes)
int write_unsigned_int(FILE *to_write_in, uint32_t to_reduce);

// Write a region to a file
void WriteRegion(FILE *file, Region_t *to_write);

//* Write Stuff

// Pixel info
struct Pixel_Node {
  int num;
  Color_t color;
  uint32_t nbs;
  struct aretes *nexts;
};
typedef struct Pixel_Node Pixel_Node_t;
typedef Pixel_Node_t *Pt_Pixel;
// Push a pixel to the nexts list
void add_next(Pt_Pixel node, int next_pixel);
// Get pixel 4-Nexts
void set_suivants(Pt_Pixel px);
// Debug Display
void Display_Pixel(Pixel_Node_t *to_print);

// Global Nodes
struct G_node {
  int vu;
  Region_t *reg_id;
  Pt_Pixel node;
};
typedef struct G_node G_node_t;
typedef struct G_node *legraphe;
struct aretes {
  Pt_Pixel pt;
  float poids; // Unused
  struct aretes *suiv;
};
typedef struct aretes aretes;
typedef struct aretes *ptarete;

void process_pixel(Region_t *to_build, int current_px, list_t *stack);
void find_bords(Region_t *to_build, int start);

double get_file_size(FILE *fp);


#define Comp_h
#endif // Comp_h
