/******************************************************************************
FileName: object_manager.cpp
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "object_manager.h"

namespace 
{
	#define ENEMY_OFFSET_Y 4.0f
	#define ENEMY_HEALTH_SIZE 0.27f
	#define ENEMY_TURRET_SIZE 10.7f
	#define ENEMY_HEALTH_EACH_DISTANCE 1.8f
	#define HEALTH_OFFSET_Y 1.0f
	#define ENEMY_SlOW_SIZE 10.f
}

void object_manager::DestroyObjects()
{
    delete[] EnemyChasers;
    delete[] EnemyTurrets;
	delete[] EnemySlow;
	delete[] ObstacleChasers;
}

void object_manager::DrawObjects(WEMtx44& _transform, 
				int BlankTxt, int EnemyChaserTxt, int EnemyTurretTxt, int EnemySlowTxt, float TileMapWidth,
				point Offset)
{
	//Draw Chaser Enemies
	for (int Index = 0; Index < NumChaser; Index++)
	{
		if(EnemyChasers[Index].Visible)
		{
			//Draw 
			WEGraphics::SetTexture(EnemyChaserTxt);
			if(EnemyChasers[Index].State == 2)
				WEGraphics::SetTextureColor(0xffe545f7);
			else
				WEGraphics::SetTextureColor(0xffffffff);
			WEMtx44::MakeTransform(_transform, EnemyChasers[Index].DirX * 2.0f * EnemyChasers[Index].Width, 2.0f * EnemyChasers[Index].Width,
				0.0f,
				EnemyChasers[Index].Pos.X, EnemyChasers[Index].Pos.Y + ENEMY_OFFSET_Y,
				0.0f);
			WEGraphics::Draw(_transform);

			//Draw Health Bar
			WEGraphics::SetTexture(BlankTxt);
			for(int HealthIndex = 0; HealthIndex < EnemyChasers[Index].Health; HealthIndex++)
			{
				WEGraphics::SetTextureColor(254,
											85,
											102, 
											254);
				WEMtx44::MakeTransform(_transform, EnemyChasers[Index].Width * ENEMY_HEALTH_SIZE, EnemyChasers[Index].Width * ENEMY_HEALTH_SIZE,
					0.0f,
					EnemyChasers[Index].Pos.X - 0.4f * EnemyChasers[Index].Width + ENEMY_HEALTH_EACH_DISTANCE * HealthIndex, EnemyChasers[Index].Pos.Y - HEALTH_OFFSET_Y,
					0.0f);
				WEGraphics::Draw(_transform);
			}

			// if(EnemyChasers[Index].Bullet.Visible)
			// {
			// 	WEGraphics::SetTextureColor(0xff6565ff);				
			// 	WEMtx44::MakeTransform(_transform, EnemyChasers[Index].Bullet.Width, EnemyChasers[Index].Bullet.Height,
			// 		0.0f,
			// 		EnemyChasers[Index].Bullet.Pos.X, EnemyChasers[Index].Bullet.Pos.Y + ENEMY_OFFSET_Y,
			// 		0.0f);
			// 	WEGraphics::Draw(_transform);
			// }
		}
	}

	//Draw Slow Enemies
	for (int Index = 0; Index < NumSlow; Index++)
	{
		if (EnemySlow[Index].Visible)
		{
			//Draw 
			WEGraphics::SetTexture(EnemySlowTxt);
			if (EnemySlow[Index].State == 2)
				WEGraphics::SetTextureColor(0xffe545f7);
			else
				WEGraphics::SetTextureColor(0xffffffff);
			WEMtx44::MakeTransform(_transform, ENEMY_SlOW_SIZE, ENEMY_SlOW_SIZE,
				0.0f,
				EnemySlow[Index].Pos.X, EnemySlow[Index].Pos.Y + ENEMY_OFFSET_Y,
				0.0f);
			WEGraphics::Draw(_transform);

			//Draw Health Bar
			WEGraphics::SetTexture(BlankTxt);
			for (int HealthIndex = 0; HealthIndex < EnemySlow[Index].Health; HealthIndex++)
			{
				WEGraphics::SetTextureColor(254,
					85,
					102,
					254);
				WEMtx44::MakeTransform(_transform, EnemySlow[Index].Width * ENEMY_HEALTH_SIZE, EnemySlow[Index].Width * ENEMY_HEALTH_SIZE,
					0.0f,
					EnemySlow[Index].Pos.X - 0.4f * EnemySlow[Index].Width + ENEMY_HEALTH_EACH_DISTANCE * HealthIndex, EnemySlow[Index].Pos.Y - HEALTH_OFFSET_Y,
					0.0f);
				WEGraphics::Draw(_transform);
			}

			// if(EnemyChasers[Index].Bullet.Visible)
			// {
			// 	WEGraphics::SetTextureColor(0xff6565ff);				
			// 	WEMtx44::MakeTransform(_transform, EnemyChasers[Index].Bullet.Width, EnemyChasers[Index].Bullet.Height,
			// 		0.0f,
			// 		EnemyChasers[Index].Bullet.Pos.X, EnemyChasers[Index].Bullet.Pos.Y + ENEMY_OFFSET_Y,
			// 		0.0f);
			// 	WEGraphics::Draw(_transform);
			// }
		}
	}

	//Draw Turret Enemies
	for (int Index = 0; Index < NumTurret; Index++)
	{
		if(EnemyTurrets[Index].Visible)
		{
			WEGraphics::SetTexture(EnemyTurretTxt);			
			WEGraphics::SetTextureColor(0xffffffff);
			WEMtx44::MakeTransform(_transform, ENEMY_TURRET_SIZE, ENEMY_TURRET_SIZE  * 2.0f,
				0.0f,
				EnemyTurrets[Index].Pos.X, EnemyTurrets[Index].Pos.Y + ENEMY_OFFSET_Y,
				0.0f);
			WEGraphics::Draw(_transform);

			WEGraphics::SetTexture(BlankTxt);
			
			for(int HealthIndex = 0; HealthIndex < EnemyTurrets[Index].Health; HealthIndex++)
			{
				WEGraphics::SetTextureColor(254,
											85,
											102, 
											254);
				WEMtx44::MakeTransform(_transform, EnemyTurrets[Index].Width * ENEMY_HEALTH_SIZE, EnemyTurrets[Index].Width * ENEMY_HEALTH_SIZE,
					0.0f,
					EnemyTurrets[Index].Pos.X - 0.4f * EnemyTurrets[Index].Width + ENEMY_HEALTH_EACH_DISTANCE * HealthIndex, EnemyTurrets[Index].Pos.Y - 2.2f * HEALTH_OFFSET_Y,
					0.0f);
				WEGraphics::Draw(_transform);
			}

			if(EnemyTurrets[Index].Bullet.Visible)
			{
				WEGraphics::SetTextureColor(0xff6565ff);
				WEMtx44::MakeTransform(_transform, EnemyTurrets[Index].Bullet.Width, EnemyTurrets[Index].Bullet.Height,
					0.0f,
					EnemyTurrets[Index].Bullet.Pos.X, EnemyTurrets[Index].Bullet.Pos.Y + ENEMY_OFFSET_Y,
					0.0f);
				WEGraphics::Draw(_transform);
			}
		}
	}

	for (int Index = 0;
		Index < NumObstacleChaser;
		++Index)
	{
		WEGraphics::SetTextureColor(0xffffffff);
		WEGraphics::SetTexture(BlankTxt);
		WEMtx44::MakeTransform(_transform, ObstacleChasers[Index].Width, ObstacleChasers[Index].Height,
			0.0f,
			ObstacleChasers[Index].Pos.X, ObstacleChasers[Index].Pos.Y,
			0.0f);

		WEGraphics::Draw(_transform);

		if (ObstacleChasers[Index].State == 2)
		{
			WEGraphics::SetTextureColor(0xffffffff);
			WEGraphics::SetTexture(BlankTxt);
			WEMtx44::MakeTransform(_transform, TileMapWidth, 15.0f * ObstacleChasers[Index].AttackingTime,
				0.0f,
				ObstacleChasers[Index].Pos.X, ObstacleChasers[Index].Pos.Y,
				0.0f);

			WEGraphics::Draw(_transform);
		}
	}
}

bool object_manager::DidEnemyDied(void)
{
	bool Result = true;
	if(NumChaser + NumTurret + NumSlow == 0)
	{
		Result = true;
	}
	else
	{
		for (int Index = 0; Index < NumChaser; Index++)
		{
			if(EnemyChasers[Index].Visible)
				Result = false;
		}

		for (int Index = 0; Index < NumTurret; Index++)
		{
			if(EnemyTurrets[Index].Visible)
				Result = false;
		}
		for (int Index = 0; Index < NumSlow; Index++)
		{
			if (EnemySlow[Index].Visible)
				Result = false;
		}
	}
	return Result;
}

void object_manager::KillAllEnemies(void)
{
		for (int Index = 0; Index < NumChaser; Index++)
		{
			EnemyChasers[Index].Health = 0;
		}

		for (int Index = 0; Index < NumTurret; Index++)
		{
			EnemyTurrets[Index].Health = 0;			
		}
		for (int Index = 0; Index < NumSlow; Index++)
		{
			EnemySlow[Index].Health = 0;
		}
}
