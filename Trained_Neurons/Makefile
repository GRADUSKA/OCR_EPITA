# Makefile

CC = gcc
CFLAGS = -g -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: main

SRC = neuron_utils.c mat_utils.c zero_and_one.c main.c
OBJ = ${SRC:.c=.o}
EXE = main


main: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@  $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@  $(LDLIBS)

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
