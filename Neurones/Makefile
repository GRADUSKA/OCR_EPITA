# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 -lm
LDFLAGS =
LDLIBS =

all: main

main: 
	$(CC) -c $(SRC)
	$(CC) -o $(CFLAGS) $(OBJ)

SRC = neuron_utils.c mat_utils.c main.c
OBJ = ${SRC:.c=.o}
EXE = ${main.c:.c=}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
