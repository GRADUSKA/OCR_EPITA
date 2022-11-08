# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -g -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -lm
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: detection cases

SRC =  detection.c use.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

detection: detection.o use.o
cases: cases.c cases.o
.PHONY: clean

clean:
	${RM} *.o
	${RM} ${EXE}
	${RM} *.bmp

# END
