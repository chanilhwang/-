/******************************************************************************/
/*!
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.

\file   	GameStateManager.h
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

#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include "GameStateList.h"
typedef void(*Func)(void);

States GetPreviousState();
States GetCurrentState();
States GetNextState();

void SetPreviousState(States Prev);
void SetCurrentState(States Cur);
void SetNextState(States Next);

Func GetLoad();
Func GetInitialize();
Func GetUpdate();
Func GetDraw();
Func GetFree();
Func GetUnload();

//GSM functions
void GSM_Initialize(States _Current);
void GSM_Update();

#endif //GAMESTATEMANAGER_H