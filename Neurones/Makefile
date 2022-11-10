# Makefile

CC = gcc
CFLAGS = -g -Wall -Wextra -O3

all: main

SRC = neuron_utils.c mat_utils.c main.c
OBJ = ${SRC:.c=.o}
EXE = main


main: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -lm

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
