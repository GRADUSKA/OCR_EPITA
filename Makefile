# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = `pkg-config --cflags gtk+-3.0 sdl2 SDL2_image` -Wall -O3 -lm
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0 sdl2 SDL2_image` -lm

SRC =  Graphic_Interface/interface.c Image_Processing/image_process.c Image_Processing/canny.c Image_Processing/Gaussian_blur.c Image_Processing/grayscale.c Image_Processing/rotation.c Image_Processing/use.c
OBJ = ${SRC:.c=.o}
EXE = final

all: ${EXE}

${EXE}: ${OBJ}
	$(CC) $(CFLAGS) $^ -o $@  $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@  $(LDLIBS)


.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
# END
