# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -g -Wall -Wextra -O3 -lm
LDFLAGS =

all: main

SRC = neuron_utils.c mat_utils.c main.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}


main: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
