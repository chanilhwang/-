/*------------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:Lab.cpp
Purpose:use 1darray like 2darray
Project:Lab2
Author:c.hwang@digipen.edu / Hwang Chan Il
Created:2019/03/20
------------------------------------------------------------------------*/
#include <iostream>

char **make2D(int width, int height)
{
	unsigned int size_of_int_over_char = sizeof(int) / sizeof(char);
	int* pointer = reinterpret_cast<int*>(new char[width * height + height * size_of_int_over_char]);

	for (int i = 0; i < height; i++)
	{
		pointer[i] = reinterpret_cast<int>(reinterpret_cast<char*>(pointer) + height * size_of_int_over_char + width * i);
	}

	return reinterpret_cast<char**>(pointer);
}

int main(){
	int width = 6;
	int height = 10;

	char **board = make2D(width, height);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if ((i%width) == (j%height))
			{
				if (i > j)
				{
					board[i][j] = 'a' + i;
				}
				else
				{
					board[i][j] = 'a' + j;
				}
			}
			else
			{
				board[i][j] = '=';
			}
		}
	}

	for (int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width; j++) 
		{
			std::cout << board[i][j] << ' ';
		}
		std::cout << std::endl;
	}

	return 0;
}