/******************************************************************************
FileName: advanced_enemy.h
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef ADVANCED_ENEMY_H

#include "enemy.h"

class enemy_Slow : public enemy
{
public:
	int State;
	//state 1 : Pause
	//state 2 : Chase
	float Timer;


	void Initialize(point InitPos, float TileWidth, float TileHeight, int _CountX, int _CountY, point DownLeftPos);
	void Move(float);
	void Pause(float);
	void EnemySlowProcess(object* Player, float dt, tile_map*);
};

#define ADVANCED_ENEMY_H
#endif