# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -g -Wall -Wextra -O3  `pkg-config --cflags sdl2 SDL2_image` -lm
LDFLAGS = -g 
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: result_grid

SRC =  use.c result_grid.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

result_grid: result_grid.o use.o
.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} *.bmp
# END
