/******************************************************************************
FileName: tile_info.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef TILE_INFO_H

#include "hreaper_general.h"

class tile_map
{
public:
	int CountX;
	int CountY;
	point DownLeftPos;
	float TileWidth;
	float TileHeight;
	int *Tiles;

	void Initialize(int _Row, int _Column, point InitDownLeftPos, 
					float InitTileWidth, float InitTileHeight, 
					int *PointerToTiles);
};

#define TILE_INFO_H
#endif