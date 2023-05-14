/******************************************************************************/
/*!
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.

\file   	System.cpp
\author		Hwang Chan IL
\par		c.hwang
\par    	tktlssla803@gmail.com
\par    	CS230
\par    	Assignment_GSM
\date   	5/24/2017

A system handler should be implemented.
(In later projects, the system will handle initializing the input manager,
the graphics manager, etc.)
*/
/******************************************************************************/

#include "System.h"
#include "FileOutput.h"

void System_Initialize()
{
	File_Created();
	File_Append("System:Initialize");
}

void System_Exit()
{
	File_Append("System:Exit");
	File_Closed();
}