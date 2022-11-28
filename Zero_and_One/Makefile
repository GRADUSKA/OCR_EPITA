# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: zero_and_one

SRC = zero_and_one.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

zeroandone: zero_and_one.o
.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} *.txt

# END
