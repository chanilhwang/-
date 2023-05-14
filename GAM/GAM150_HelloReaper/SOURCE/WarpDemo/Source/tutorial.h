/******************************************************************************
FileName: tutorial.h
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef TUTORIAL_H

#include "hreaper_general.h"
#include "level_info.h"

struct checkingList
{
	int Move1 = 3;
	int Move2 = 3;
	int Attacking = 3;
	int Interact = 3;
	bool Lock = false;

	bool IsEnergyExhausted = false;
	float SignLife = 3.0f;
	#define SIGNLIFE 3.0f
};

void tutorialLoad();
void tutorialInit();
void tutorialUpdate(float dt);
void tutorialShutdown();
void tutorialUnload();


#define TUTORIAL_H
#endif