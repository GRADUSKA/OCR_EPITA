#include <stdio.h>
#include <stdlib.h>

#pragma once

int is_possible(char sudoku[], int i, int j, char value);
int solver(char sudoku[]);
int fill_sudoku(char* name, char sudoku[]);
void file_result(char *name, char sudoku[]);
int solver1(char* name);
