#include "./Lib/ima.h"
#include <stdlib.h>

Image *image;

int Init(char *s) {
  image = (Image *)malloc(sizeof(Image));
  if (image == NULL) {
    fprintf(stderr, "Out of memory\n");
    return (-1);
  }
  if (ImageLoad_PPM(s, image) == -1)
    return (-1);
  printf("tailles %d %d\n", (int)image->sizeX, (int)image->sizeY);
  return (EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage : Compress FileName\n");
    exit(0);
  }
  Init(argv[1]); 
}