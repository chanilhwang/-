/******************************************************************************
FileName: tile.cpp
Project: Hello Reaper
Author: Yongwon, Cho (y.cho)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "tile.h"
#include "collision.h"

namespace
{
	#define ID_DEATHTILE 2
	#define ID_MOVETILE 3
	#define ID_TRAPTILE 4
	#define COLLIDERANGE 15.8f
	#define CENTERRANGE 0.5f
	#define MOVELENGTH 1.0f
	#define MOVETIME 1.3f
	#define TRAPTIME 1.0f
	#define MOVESPEED 34.0f
	#define DASH_COOLDOWN 0.28f
}

//General SpecialTile
//Initailize and define the type of tiles. Set 2 as DeathTile, 3 as MoveTile.
void special_tile::Initialize(point TilePos,
							  float TileWidth,float TileHeight,
							  int _CountX, int _CountY,
							  int InputID)
{
	Pos.X = (TilePos.X - _CountX / 2) * TileWidth;
	Pos.Y = (TilePos.Y - _CountY / 2) * TileHeight;

	Width = TileWidth;
	Height = TileHeight;

	TypeID = InputID;
	State = false;
}

////Check the collision between the player and tile.
//bool special_tile::CollideForPlayer(player* Player)
//{
//	bool Collided = false;
//
//	if (TypeID > 1)
//	{
//			if(Player->PlayerTileX == PosX && Player->PlayerTileY == PosY)
//			Collided = true;
//			State = true;
//	}
//	
//	return Collided;
//}

bool special_tile::CollideForObject(object *Target)
{
	bool Collided = false;

	if (TypeID > 1)
	{
		if ((Target->Pos.X - this->Pos.X) <= (Width * 0.5f) &&
			(Target->Pos.X - this->Pos.X) >= -(Width * 0.5f) &&
			(Target->Pos.Y - this->Pos.Y) <= (Height * 0.5f) &&
			(Target->Pos.Y - this->Pos.Y) >= -(Height * 0.5f))
		{
			Collided = true;
			State = true;
		}
	}

	return Collided;
}

bool special_tile::CollideAtCenter(object *Target)
{
	bool Collide = false;

	if (TypeID > 1)
	{
		if ((Target->Pos.X - this->Pos.X) <= (Width * 0.1f) &&
			(Target->Pos.X - this->Pos.X) >= -(Width * 0.1f) &&
			(Target->Pos.Y - this->Pos.Y) <= (Height * 0.1f) &&
			(Target->Pos.Y - this->Pos.Y) >= -(Height * 0.1f))
		{
			Collide = true;
			State = true;
		}
	}
	return Collide;
}

bool special_tile::CollideAtCenter(player *Player)
{
	bool Collide = false;
	
	if (TypeID > 1)
	{
		if (Player->MovingTime <= 0)
		{
			State = false;
			if ((Player->Pos.X - this->Pos.X) <= (Width * 0.5f) &&
				(Player->Pos.X - this->Pos.X) >= -(Width * 0.5f) &&
				(Player->Pos.Y - this->Pos.Y) <= (Height * 0.5f) &&
				(Player->Pos.Y - this->Pos.Y) >= -(Height * 0.5f))
			{	
				Collide = true;
				State = true;
			}
		}
	}
	return Collide;
}

//DeathTile
void death_tile::DeathTileProcess(object *Target)
{
	if (CollideForObject(Target) == true)
	{
		Target->Health = 0;
		Target->Visible = false;
	}
}

//MoveTile
//Initialize the MoveTile. It contains direction of the tile.
void move_tile::MoveInitialize(point TilePos,
							  float TileWidth,float TileHeight,
							  int _CountX, int _CountY,
							  int InputID, point MoveDestination)
{
	Initialize(TilePos, TileWidth, TileHeight, _CountX, _CountY, InputID);
	Destination = MoveDestination;
}

void move_tile::PrepareForObject(object *Target, float dt)
{
	Target->dX = 0.0f;
	Target->dY = 0.0f;
	Target->dX = Destination.X;
	Target->dY = Destination.Y;
}

void move_tile::PrepareForPlayer(player *Player, float dt)
{
	Player->MovingTime = 0.1f;
	Player->Pos = Pos;
	Player->dX = 0;
	Player->dY = 0;
	switch (TypeID) {
	case 3:
		Player->dX = -1.0f;
		Player->DirX = -1;
		Player->DashDirX = -1;
		break;
	case 4:
		Player->dX = 1.0f;
		Player->DirX = 1;
		Player->DashDirX = 1;
		break;
	case 5:
		Player->dY = 1.0f;
		Player->DirY = 1;
		Player->DashDirY = 1;
		break;
	case 6:
		Player->dY = -1.0f;
		Player->DirY = -1;
		Player->DashDirY = -1;
		break;
	default:
		break;
	}
	
	//Player->Destination.X = Destination.X;
	//Player->Destination.Y = Destination.Y;
	Player->dX *= Width;
	Player->dY *= Height;
	Player->MovingTime = DASH_COOLDOWN;

	Player->Destination.X = Player->Pos.X + Player->dX;
	Player->Destination.Y = Player->Pos.Y + Player->dY;
	Player->Start = Player->Pos;
	Player->State = 1;
	Player->PlayerIllusionCount = 0;
	Player->TextCoord = 0.0f;
}

//Move process for objects. It stop the objects and move the direction.
void move_tile::MoveForObject(object *Target, float dt)
{
	object* TargetE = nullptr;
	float MoveTime = 0;
	if (CollideAtCenter(Target))
	{
		TargetE = Target;
		MoveTime = MOVETIME;
	}
	if (MoveTime >= 0.0f)
	{
		PrepareForObject(TargetE, dt);
		TargetE->Pos.X += MOVESPEED * TargetE->dX * dt;
		TargetE->Pos.Y += MOVESPEED * TargetE->dY * dt;
		MoveTime -= dt;
	}
	if (MoveTime <= 0.0f)
	{
		TargetE = 0;
	}
}

//Move process for player. It stop the player and move the direction.
void move_tile::MoveForPlayer(player *Player, float dt)
{
	
	if (CollideAtCenter(Player))
	{
		PrepareForPlayer(Player, dt);

		Player->MovingTime = DASH_COOLDOWN;
		Player->DashType = 1;
	}
}