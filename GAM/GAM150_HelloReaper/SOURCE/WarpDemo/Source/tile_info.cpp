/******************************************************************************
FileName: tile_info.cpp
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "tile_info.h"

void tile_map::Initialize(int _Row, int _Column,
						point InitDownLeftPos,
						float InitTileWidth, float InitTileHeight,
						int *PointerToTiles)
{
	CountX = _Row;
	CountY = _Column;
	DownLeftPos = InitDownLeftPos;
	TileWidth = InitTileWidth;
	TileHeight = InitTileHeight;
	Tiles = PointerToTiles;
}