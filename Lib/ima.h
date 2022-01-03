#ifndef Ima_h
#define Ima_h
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __APPLE__
#include <GL/gl.h>
#else
/* pour mac */

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#endif

struct Image {
  uint32_t sizeX;
  uint32_t sizeY;
  uint32_t image_size; // SizeX * SizeY
  GLubyte *data;       // RGB Vector
};
typedef struct Image Image;

int ImageLoad_PPM(char *filename, Image *image);
void imagesave_PPM(char *filename, Image *image);
void upsidedown(Image *img);
#endif // !Ima_h
