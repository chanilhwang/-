/******************************************************************************/
/*!
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.

\file   	Level2.cpp
\author		Hwang Chan IL
\par		c.hwang
\par    	tktlssla803@gmail.com
\par    	CS230
\par    	Assignment_GSM
\date   	5/24/2017

Each level will have its own state functions
to load, initialize, update, draw, free, and unload its data.
*/
/******************************************************************************/

#include "Level2.h"
#include "FileOutput.h"
#include "GameStateManager.h"
#include <fstream>

namespace
{
	std::ifstream fin;
	int Level2_Counter, Level2_Lives;
}

void Level2Load()
{
	fin.open("Level2_Lives.txt");
	fin >> Level2_Lives;
	fin.close();
	File_Append("Level2:Load");
}
void Level2Initialize()
{
	fin.open("Level2_Counter.txt");
	fin >> Level2_Counter;
	fin.close();
	File_Append("Level2:Initialize");
}
void Level2Update()
{
	Level2_Counter--;

	if (Level2_Counter == 0)
	{
		Level2_Lives--;
		if (Level2_Lives == 0)
		{
			SetNextState(Quit);
		}
		else
		{
			SetNextState(Restart);
		}
	}
	File_Append("Level2:Update");
}
void Level2Draw()
{
	File_Append("Level2:Draw");
}
void Level2Free()
{
	File_Append("Level2:Free");
}
void Level2Unload()
{
	File_Append("Level2:Unload");
}