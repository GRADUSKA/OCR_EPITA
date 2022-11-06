# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: grayscale rotation

SRC =  use.c grayscale.c rotation.c Gaussian_blur.c rotationbetter.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

grayscale: grayscale.o use.o
rotation: rotation.o use.o
.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} *.bmp

# END
