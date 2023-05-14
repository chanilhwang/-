/******************************************************************************
FileName: tile.h
Project: Hello Reaper
Author: Yongwon, Cho (y.cho)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once
#ifndef TILE_H

#include "hreaper_general.h"
#include "player.h"

class special_tile
{
public:
	point Pos;

	float Width;
	float Height;

	int TypeID;
	bool State;

	void Initialize(point TilePos, float TileWidth, float TileHeight,
					int _CountX, int _CountY,int InputID);
	//bool CollideForPlayer(player *Player);
	bool CollideForObject(object *Target);
	bool CollideAtCenter(object *Target);
	bool CollideAtCenter(player *Player);
};

class death_tile : public special_tile
{
public:
	void DeathTileProcess(object *Target);
};

class move_tile : public special_tile
{
public:
	point Destination;

	void MoveInitialize(point TilePos,float TileWidth, float TileHeight,
						int _CountX, int _CountY, int InputID, point Destination);
	void PrepareForObject(object *Target, float dt);
	void PrepareForPlayer(player *Player, float dt);
	void MoveForObject(object *Target, float dt);
	void MoveForPlayer(player *Player, float dt);
};

#define TILE_H
#endif