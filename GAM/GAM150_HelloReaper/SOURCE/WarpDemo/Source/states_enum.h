/******************************************************************************
FileName: state_enum.h
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef STATES_ENUM_H

enum States
{
	STATE_SPLASH_SCREEN,
    STATE_MAIN_MENU,
	STATE_TUTORIAL,
	STATE_GAME_PLAYING,
    STATE_UPGRADE_LEVEL,
    STATE_SHOP,
    STATE_ENDING
};

#define STATES_ENUM_H
#endif