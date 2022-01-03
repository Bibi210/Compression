#ifndef Decomp_h
#include "../Lib/Shared_Comp_Decomp.h"
#include <stdio.h>
#include <time.h>

// Set a pixel with a Color
void set_pixel_color(Color_t color, int pixel);
// Read an int written on 3 bytes
int read_unsigned_int(FILE *fp, uint32_t *dst);
// Read Image SizeX and SizeY
int read_image_sizes(FILE *fp);

// Read 1 Region
int ReadRegion(FILE *fp, Region_t *dst);
// Read All Regions
int ReadGraph(FILE *fp);
// Compare the Original Picture with the Decompressed Picture
void Comparaison_Img(Image *A, Image *B);

#define Decomp_h
#endif // !Decomp_h
