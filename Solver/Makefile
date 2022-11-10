CC = gcc -fsanitize=address
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -std=c99 -O0 -g
LDFLAGS = 
LDLIBS = 

OBJ = solver.o
DEP = ${OBJ:.o=.d}

all: solver

main: ${OBJ}

clean:
	${RM} ${OBJ} 
	${RM} ${DEP}
	${RM} solver
	${RM} grids/*.result

-include ${DEP}
