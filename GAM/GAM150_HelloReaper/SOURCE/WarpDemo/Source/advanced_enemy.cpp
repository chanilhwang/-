/******************************************************************************
FileName: advanced_enemy.cpp
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "advanced_enemy.h"
#include <math.h>

namespace
{
	#define	ENEMY_Slow_HEALTH 1
	#define ENEMY_ATTACK_COOLDOWN 1.3f
	#define ENEMY_Slow_MOVE_SPEED 30.f
	#define ENEMY_Slow_MOVE_TIME 0.4f
	#define ENEMY_Slow_PAUSE_TIME 2.0f
}

void enemy_Slow::Initialize(point InitPos, float TileWidth, float TileHeight, int _CountX, int _CountY, point DownLeftPos)
{
	Pos = InitPos;
	Width = TileWidth;
	Height = TileHeight;
	Health = ENEMY_Slow_HEALTH;

	State = 1;
	Timer = ENEMY_Slow_MOVE_TIME;
	Visible = true;
	id = -1;
	Bullet.Initialize(0.8 * Width, 0.8f * Height, id, 2.0f);
	Bullet.Visible = true;
	AttackCoolDown = ENEMY_ATTACK_COOLDOWN;

	dX = 0.0f;
	dY = 0.0f;
}

void enemy_Slow::Move(float dt)
{
	if (Timer < 0.0f)
	{
		State = 1;
		Timer = ENEMY_Slow_PAUSE_TIME;
	}
	else
	{
		Pos.X += ENEMY_Slow_MOVE_SPEED * dX * dt;
		Pos.Y += ENEMY_Slow_MOVE_SPEED * dY * dt;
	}
}

void enemy_Slow::Pause(float dt)
{
	if (Timer < 0.0f)
	{
		State = 2;
		Timer = ENEMY_Slow_MOVE_TIME;
	}
	else
	{
		dX = 0.0f;
		dY = 0.0f;
	}
}

void enemy_Slow::EnemySlowProcess(object* Player, float dt, tile_map* tilemap)
{
	if (HealthCheck())
	{
		if (State == 1)
		{
			Pause(dt);
		}
		else if (State == 2)
		{
			float Distance = (float)sqrt((Player->Pos.X - Pos.X) * (Player->Pos.X - Pos.X) +
										(Player->Pos.Y - Pos.Y) * (Player->Pos.Y - Pos.Y));
			point MovingVector = { (Player->Pos.X - Pos.X) / Distance,
									(Player->Pos.Y - Pos.Y) / Distance };
			dX = MovingVector.X;
			dY = MovingVector.Y;
			Move(dt);
		}

		if (AttackCoolDown > 0.0f)
		{
			AttackCoolDown -= dt;
		}
		else if (Bullet.CollideCheck(Player))
		{
			AttackCoolDown = ENEMY_ATTACK_COOLDOWN;
			Bullet.Visible = false;
		}
		else
		{
			Bullet.Visible = true;
		}

		Timer -= dt;
		Bullet.Pos = Pos;
	}
}