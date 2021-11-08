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
  unsigned long sizeX;
  unsigned long sizeY;
  GLubyte *data;
};
typedef struct Image Image;
typedef unsigned short utab[3][3][3];

int ImageLoad_PPM(char *filename, Image *image);
void imagesave_PPM(char *filename, Image *image);
void upsidedown(Image *img);