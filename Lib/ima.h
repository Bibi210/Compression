#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#define GL_SILENCE_DEPRECATION
#define __OSX__
#ifndef __OSX__
/* pour Bocal */
#include <GL/gl.h>
#else
/* pour mac */
#include <GLUT/glut.h>
#endif

struct Image {
  uint32_t sizeX;
  uint32_t sizeY;
  uint32_t image_size;
  GLubyte *data;
};
typedef struct Image Image;

int ImageLoad_PPM(char *filename, Image *image);
void imagesave_PPM(char *filename, Image *image);
void upsidedown(Image *img);