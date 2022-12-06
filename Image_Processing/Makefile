# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -g -Wall -Wextra -O3  `pkg-config --cflags sdl2 SDL2_image` -lm
LDFLAGS = -g 
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: main

SRC = main.c use.c grayscale.c rotation.c Gaussian_blur.c canny.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

main: main.o canny.o use.o rotation.o Gaussian_blur.o grayscale.o
.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} *.bmp
	${RM} *.jpg
# END
