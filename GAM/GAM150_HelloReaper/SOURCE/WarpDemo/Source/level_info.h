/******************************************************************************
FileName: level_info.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef LEVEL_INFO_H

#include "hreaper_general.h"
#include "tile_info.h"
#include "object_manager.h"
#include "tile_Manager.h"

class level_info
{
public:
	tile_map TileMap;
	point InitPlayerPos;

	void Initialize(int _Row, int _Column,
					point InitDownLeftPos,
					float InitTileWidth, float InitTileHeight,
					int *PointerToTiles, object_manager *ObjectManager, tile_manager *TileManager);
	void level_info::PositionEnemies(tile_map *TileMap, object_manager *ObjectManager);
	void level_info::PositionTiles(tile_map *TileMap, tile_manager *TileManager);
};

#define LEVEL_INFO_H
#endif