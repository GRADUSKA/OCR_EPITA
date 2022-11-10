# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -g -Wall -Wextra -O3  `pkg-config --cflags sdl2 SDL2_image` -lm
LDFLAGS = -g 
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: grayscale rotation Gaussian_blur canny

SRC =  use.c grayscale.c rotation.c Gaussian_blur.c canny.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

grayscale: grayscale.o use.o
rotation: rotation.o use.o
Gaussian_blur: Gaussian_blur.o use.o
canny: canny.o use.o
.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} *.bmp
	${RM} *.jpg
# END
