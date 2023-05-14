/******************************************************************************/
/*!
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.

\file   	GameStateManager.cpp
\author		Hwang Chan IL
\par		c.hwang
\par    	tktlssla803@gmail.com
\par    	CS230
\par    	Assignment_GSM
\date   	5/24/2017

The game state manager is responsible for switching states and
calling the correspondent state functionalities.
*/
/******************************************************************************/

#include "FileOutput.h"
#include "GameStateManager.h"
#include "Level1.h"
#include "level2.h"

namespace
{
	//Store previous, current, next state
	States PreviousState;
	States CurrentState;
	States NextState;

	//pointer to functions
	Func Load;
	Func Initialize;
	Func Update;
	Func Draw;
	Func Free;
	Func Unload;
}

States GetPreviousState()
{
	return PreviousState;
}
States GetCurrentState()
{
	return CurrentState;
}
States GetNextState()
{
	return NextState;
}

void SetPreviousState(States Prev)
{
	PreviousState = Prev;
}
void SetCurrentState(States Cur)
{
	CurrentState = Cur;
}
void SetNextState(States Next)
{
	NextState = Next;
}

Func GetLoad()
{
	return Load;
}
Func GetInitialize()
{
	return Initialize;
}
Func GetUpdate()
{
	return Update;
}
Func GetDraw()
{
	return Draw;
}
Func GetFree()
{
	return Free;
}
Func GetUnload()
{
	return Unload;
}


void GSM_Initialize(States _Current)
{
	PreviousState = _Current;
	CurrentState = _Current;
	NextState	= _Current;

	File_Append("GSM:Initialize");
}

void GSM_Update()
{
	switch (CurrentState)
	{
	case Level1:
	{
		Load = Level1Load;
		Initialize = Level1Initialize;
		Update = Level1Update;
		Draw = Level1Draw;
		Free = Level1Free;
		Unload = Level1Unload;
	} break;

	case Level2:
	{
		Load = Level2Load;
		Initialize = Level2Initialize;
		Update = Level2Update;
		Draw = Level2Draw;
		Free = Level2Free;
		Unload = Level2Unload;
	} break;
	}

	File_Append("GSM:Update");
}