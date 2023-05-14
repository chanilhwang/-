
/******************************************************************************
filename    ScanText.c
author      Matt Casanova
DP email    mcasanov@digipen.edu
course      CS120
assignment  6
due date    12/11/2016

Brief Description:
  This file contains functions for the ScanText assignment in CS120.

******************************************************************************/

#include "ScanText.h"

int CountWords(const char* string)
{
	int count = 0;
	bool iscount = true;
	for (int i = 0; string[i] != 0; i++)
	{
		if (string[i] == '\t' || string[i] == '\n' || string[i] == ' ')
		{
			iscount = true;
		}
		else if (iscount)
		{
			count += 1;
			iscount = false;
		}
	}
	return count;
}

int CountTabs(const char* string)
{
	int count = 0;
	for (int i = 0; string[i] != 0; i++)
		if (string[i] == '\t')
			count++;
	return count;
}

void CalculateLengths(const char* string, int tabSize,
					  int* stringLength, int* displayLength)
{
	*stringLength = 0;
	*displayLength = 0;

	for (int i = 0; string[i] != 0; i++)
	{
		if (string[i] == '\t')
		{
			*stringLength += 1;
			*displayLength += tabSize;
		}
		else
		{
			*stringLength += 1;
			*displayLength += 1;
		}
	}
}

int SubstituteChar(char* string, char oldChar, char newChar)
{
	int count = 0;
	for (int i = 0; string[i] != 0; i++)
		if(string[i] == oldChar)
		{
			string[i] = newChar;
			count++;
		}
	return count;
}