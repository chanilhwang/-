/******************************************************************************
FileName: tile_Manager.h
Project: Hello Reaper
Author: Yongwon, Cho (y.cho)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once
#ifndef TILE_MANAGER_H

#include "WEMtx44.h"
#include "WEGraphics.h"

#include "hreaper_intrinsics.h"
#include "tile.h"


class tile_manager
{
public:
	//tile_manager();

	death_tile* DeathTiles;
	move_tile* MoveTiles;

	int NumDeath;
	int NumMove;

	void DrawTiles(WEMtx44& _transform, int DeathTileTxt, 
		int MoveTileLeftTxt, int MoveTileRightTxt, 
		int MoveTileUpTxt, int MoveTileDownTxt, point Offset);
	void DestroyTiles();
};

#define TILE_MANAGER_H
#endif