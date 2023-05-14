/******************************************************************************
FileName: fmod_general.cpp
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "fmod_general.h"

void fmod_sound_system::MakeSound(char *SongName, bool Loop)
{
	/*
	Create a System object and initialize
	*/
	system->release();
	FMOD::System_Create(&system);

	system->getVersion(&version);

	system->init(32, FMOD_INIT_NORMAL, extradriverdata);

	system->createSound(SongName, FMOD_DEFAULT, 0, &sound1);

	if(Loop)
		sound1->setMode(FMOD_LOOP_NORMAL);
}

void fmod_sound_system::PlaySound(void)
{
	system->playSound(sound1, 0, false, &channel);
}