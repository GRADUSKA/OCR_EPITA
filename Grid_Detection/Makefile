# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -g -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image` -lm
LDFLAGS = -g
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: detection

SRC =  detection.c use.c blob.c ../Image_Processing/rotation.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

detection: detection.o use.o blob.c ../Image_Processing/rotation.o
.PHONY: clean

clean:
	${RM} *.o
	${RM} ${EXE}
	${RM} *.bmp

# END
