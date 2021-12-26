#  Makefile 
#  Auteur : Fares BELHADJ
#  Email  : amsi@ai.univ-paris8.fr
#  Date   : 23/02/2006

#  Modificiation : Dibassi Brahima
#  Commentaire : Modfier Pour Projet
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
CFLAGS := -Wall -g -finline-functions -funroll-loops  -mtune=native -flto -O3
LDFLAGS = -lm $(GL_LDFLAGS)
EXE := Decomp Comp Afficher

all: $(EXE)

Comp: Lib/Src/list.o Lib/Src/ppm.o Lib/Src/Shared_Comp_Decomp.o
Decomp: Lib/Src/list.o Lib/Src/ppm.o Lib/Src/Shared_Comp_Decomp.o
Afficher: Lib/Src/ppm.o 


list.o: Lib/list.h
ppm.o: Lib/ima.h
Shared_Comp_Decomp.o: Lib/Shared_Comp_Decomp.h

clean:
	$(RM) $(EXE) *~ $(shell find . -name "*.o")
	$(RM) $(EXE) *~ $(shell find . -name "*.Compressed")
	$(RM) -rf *.dSYM
	$(RM) -rf demo_comp.ppm

.PHONY: all clean