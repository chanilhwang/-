/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: matrix.c
Purpose: to create and delete matrix
Language: c
Platform: Windows 64bit
Project: c.hwang_CS225_lab2
Author: Hwnag Chan Il
Creation date: 9/12/2018
- End Header ----------------------------------------------------------------
*/

#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

Matrix matrix_create(int num_rows, int num_columns)
{
    int i;
    Matrix result = (int**)malloc(num_rows * sizeof(int*));
    for(i = 0; i < num_rows; i++)
    {
        result[i] = (int*)malloc(num_columns * sizeof(int));
    }

    return result;
}

void matrix_add(Matrix m1, Matrix m2, Matrix result, int num_rows, int num_columns)
{
    int i, j;
    for(i = 0; i < num_rows; i++)
    {
        for(j = 0; j < num_columns; j++)
        {
            result[i][j] = m1[i][j] + m2[i][j];
        }
    }
}

Matrix matrix_transpose(Matrix m, int num_rows, int num_columns)
{
    int i, j;
    Matrix result = matrix_create(num_columns, num_rows);

    for(i = 0; i < num_columns; i++)
    {
        for(j = 0; j < num_rows; j++)
        {
            result[i][j] = m[j][i];
        }
    }

    return result;
}

void matrix_delete(Matrix m, int num_rows)
{
    int i;
    for(i = 0; i < num_rows; i++)
    {
        free(m[i]);
    }
    free(m);
}

void  matrix_print( Matrix m, int num_rows, int num_columns ) {
    int i, j;
    for ( i=0; i<num_rows; ++i ) {
        for ( j=0; j<num_columns; ++j ) {
            printf( "%4i ", m[i][j] );
        }
        printf( "\n" );
    }
}

void matrix_delete_row(Matrix m, int r, int num_columns)
{
    int i;
    int* buf = m[r];
    for(i = r;i < (num_columns - 1) ; i++)
    {
        m[i] = m[i+1];
    }
    m[num_columns - 1] = buf;
    free(m[num_columns - 1]);
}

void matrix_delete_column(Matrix m, int c, int num_rows, int num_columns)
{
    int i, j;
    for(i=0; i < num_rows; i++)
    {
        for(j = c; j < num_columns - 1; j++)
        {
            m[i][j] = m[i][j+1];
        }
    }
}