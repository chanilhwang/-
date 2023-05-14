/******************************************************************************
FileName: object_manager.h
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef OBJECT_MANAGER_H

#include "WEMtx44.h"
#include "WEGraphics.h"

#include "hreaper_general.h"
#include "hreaper_intrinsics.h"
#include "enemy.h"
#include "obstacle.h"
#include "advanced_enemy.h"
#include "obstacle.h"

class object_manager
{
public:
    enemy_chaser* EnemyChasers;
    enemy_turret* EnemyTurrets;
	enemy_Slow* EnemySlow;
	obstacle_chaser* ObstacleChasers;

	int NumChaser;
	int NumTurret;
	int NumSlow;
	int NumObstacleChaser;

	void DrawObjects(WEMtx44& _transform, int BlankTxt, 
					 int EnemyTurretTxt, int EnemyChaserTxt, int EnemySlowTxt, float,
					 point Offset);
    void DestroyObjects();
    bool DidEnemyDied(void);
	void KillAllEnemies(void);
};

#define OBJECT_MANAGER_H
#endif