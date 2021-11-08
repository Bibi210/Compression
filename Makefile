#  Makefile 
#  Auteur : Fares BELHADJ
#  Email  : amsi@ai.univ-paris8.fr
#  Date   : 23/02/2006

#  Modificiation : Mehdi Aghayani
#  Email  : mehdi.aghayani@gmail.com
#  Commentaire : modifi� afin de fonctionner avec OpenGL et MacOS X et libpng
#  
#  Universit� Paris VIII

SHELL = /bin/sh
#definition des commandes utilisees
MACHINE= $(shell uname -s)
#declaration des options du compilateur
#PG_FLAGS = -DOPENGL_1_5

ifeq ($(MACHINE), Darwin)
GL_LDFLAGS = -framework OpenGL -framework GLUT -framework Cocoa
else
GL_LDFLAGS = -lGL -lglut -lGLU
endif

CC = gcc
CFLAGS := -Wall
LDFLAGS = -lm $(GL_LDFLAGS)
EXE := Decomp Comp

all: $(EXE)

Comp: Lib/Src/list.o Lib/Src/ppm.o
Decomp: Lib/Src/list.o Lib/Src/ppm.o 

list.o: Lib/list.h
ppm.o: Lib/ima.h


clean:
	$(RM) $(EXE) *~ $(shell find . -name "*.o")

.PHONY: all clean