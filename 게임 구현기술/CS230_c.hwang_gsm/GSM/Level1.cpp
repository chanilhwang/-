/******************************************************************************/
/*!
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.

\file   	Level1.cpp
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

#include "Level1.h"
#include "FileOutput.h"
#include "GameStateManager.h"
#include <fstream>

namespace
{
	std::ifstream fin;
	int Level1_Counter;
}

void Level1Load()
{
	fin.open("Level1_Counter.txt");
	fin >> Level1_Counter;
	fin.close();
	File_Append("Level1:Load");
}
void Level1Initialize()
{
	File_Append("Level1:Initialize");
}
void Level1Update()
{
	Level1_Counter--;

	if (Level1_Counter == 0)
		SetNextState(Level2);

	File_Append("Level1:Update");
}
void Level1Draw()
{
	File_Append("Level1:Draw");
}
void Level1Free()
{
	File_Append("Level1:Free");
}
void Level1Unload()
{
	File_Append("Level1:Unload");
}