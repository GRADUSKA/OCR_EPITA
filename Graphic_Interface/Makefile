# Makefile

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -O3
LDLIBS = `pkg-config --libs gtk+-3.0`


all: interface

SRC =  interface.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

interface: interface.o
.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} *.bmp
	${RM} *.jpg
# END
