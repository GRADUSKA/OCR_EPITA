# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -g -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image` -lm
LDFLAGS = -g
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: detection

SRC =  detection.c ../Image_Processing/use.c blob.c ../Image_Processing/image_process.c ../Image_Processing/Gaussian_blur.c \
../Image_Processing/canny.c ../Image_Processing/grayscale.c ../Image_Processing/rotation.c ../Image_Processing/use.c




OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

detection: detection.o ../Image_Processing/use.o blob.o ../Image_Processing/image_process.o ../Image_Processing/Gaussian_blur.o \
../Image_Processing/canny.o ../Image_Processing/grayscale.o ../Image_Processing/rotation.o ../Image_Processing/use.o

.PHONY: clean

clean:
	${RM} *.o
	${RM} ${EXE}
	${RM} *.bmp
	${RM} ../Image_Processing/*.o

# END
