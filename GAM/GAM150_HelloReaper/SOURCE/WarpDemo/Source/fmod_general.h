/******************************************************************************
FileName: fmod_general.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#ifndef FMOD_GENERAL_H

#include "fmod.hpp"

class fmod_sound_system
{
public:
	FMOD::System     *system;
	FMOD::Sound      *sound1;
	FMOD::Channel    *channel = 0;
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata = 0;
    
	void MakeSound(char *SongName, bool Loop);
	void PlaySound(void);
};

#define FMOD_GENERAL_H
#endif