#include <stdio.h>
#include <stdlib.h>


//check if it's possible to place the value
int is_possible(char sudoku[], int i, int j, char value)
{

    for(int a = 0; a < 9; a++) //check in column
    {
        if(sudoku[a*9+j] == value)
        {
            return 0;
        }
    }

    for(int b = 0; b < 9; b++) //check in row
    {
        if(sudoku[i*9+b] == value)
        {
            return 0;
        }
    }

    i = i - i % 3;
    j = j - j % 3;
    for(int k = i; k < i + 3; k++) // check in cube 3x3
    {
        for(int l = j; l < j + 3; l++)
        {
            if(sudoku[k*9+l] == value)
            {
                return 0;
            }
        }
    }
    return 1; //the placement is possible !!!
}


//the solver using backtracking
int solver(char sudoku[])
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(sudoku[i*9+j] == '.')
            {
                for(char n = '1'; n <= '9'; n++)
                {
                    if(is_possible(sudoku, i, j, n) == 1)
                    {
                        sudoku[i*9+j] =  n;
                        if(solver(sudoku) == 1)
                        {
                            return 1;
                        }
                        sudoku[i*9+j] = '.';
                    }
                }
                return 0;
            }
        }
    }
    return 1;
}


//fill the sudoku with the sudoku in the file
int fill_sudoku(char* name, char sudoku[])
{
    FILE *fp;

    fp = fopen(name, "r");
    if(fp == NULL)
    {
        printf("Erreur fopen\n");
        return 1;
    }

    char c;
    int i = 0;
    while (i < 81 && (c = fgetc(fp)) != EOF)
    {
        if (c != ' ' && c != '\n')
        {
            sudoku[i] = c;
            i++;
        }
    }

    fclose(fp);
    return 0;
}


//write the sudoku result in a file .result
void file_result(char *name, char sudoku[])
{
    char result[] = ".result";

    int z = 0;
    while(name[z] != '\0')
    {
        z++;
    }

    int p = 0;
    while(result[p] != '\0')
    {
        name[z] = result[p];
        p++;
        z++;
    }
    name[z] = '\0';



    FILE *file = fopen(name, "w");

    for(int j = 0; j < 9; j++)
    {
        for(int k = 0; k < 9; k++)
        {
            if(k == 3 || k == 6 )
            {
                fputc(' ', file);
            }
            fputc(sudoku[j*9+k], file);
        }
        fputc('\n', file);
        if(j == 2 || j == 5)
        {
            fputc('\n', file);
        }
    }

}


int solver1(char* name)
{
    char sudoku[81];

    fill_sudoku(name, sudoku);
    solver(sudoku);
    file_result(name, sudoku);

    return 0;

}

