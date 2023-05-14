/******************************************************************************
FileName: level_info.cpp
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "level_info.h"

namespace
{
	#define CHASER_TILE_ID -1 
	#define TURRET_TILE_ID -2 
	#define SLOW_TILE_ID -3
	#define OBSTACLE_CHASER_TILE_ID -5
	#define DEATH_TILE_ID 2
	#define MOVE_TILE_ID_L 3
	#define MOVE_TILE_ID_R 4
	#define MOVE_TILE_ID_U 5
	#define MOVE_TILE_ID_D 6
	#define PLAYER_TILE_ID 7
	#define TRAP_TILE_ID 8	
}

//Goes through the loop again and initialize the enemies
void level_info::PositionEnemies(tile_map *_TileMap, object_manager *ObjectManager)
{
	int ChaserIndex = 0;
	int TurretIndex = 0;
	int SlowIndex = 0;
	int ObstacleChaserIndex = 0;
	for (int Row = 0;
		Row < _TileMap->CountY;
		Row++)
	{
		for (int Column = 0;
			Column < _TileMap->CountX;
			Column++)
		{
			int TileID = _TileMap->Tiles[Row * _TileMap->CountX + Column];

			if(TileID == CHASER_TILE_ID)
			{
				ObjectManager->EnemyChasers[ChaserIndex].Initialize({(float)Row, (float)Column},
														_TileMap->TileWidth, _TileMap->TileHeight,
														_TileMap->CountX, _TileMap->CountY,
														_TileMap->DownLeftPos);
				++ChaserIndex;
			}
			else if(TileID == TURRET_TILE_ID)
			{
				ObjectManager->EnemyTurrets[TurretIndex].Initialize({(float)Row, (float)Column},
																	_TileMap->TileWidth, _TileMap->TileHeight,
																	_TileMap->CountX, _TileMap->CountY,
																	_TileMap->DownLeftPos);
				++TurretIndex;
			}
			else if(TileID == SLOW_TILE_ID)
			{
				ObjectManager->EnemySlow[SlowIndex].Initialize({(float)Row, (float)Column},
																	_TileMap->TileWidth, _TileMap->TileHeight,
																	_TileMap->CountX, _TileMap->CountY,
																	_TileMap->DownLeftPos);
				++SlowIndex;
			}
			else if (TileID == OBSTACLE_CHASER_TILE_ID)
			{
				ObjectManager->ObstacleChasers[ObstacleChaserIndex].Initialize({ (float)Row, (float)Column },
					_TileMap->TileWidth, _TileMap->TileHeight,
					_TileMap->CountX, _TileMap->CountY);
				++ObstacleChaserIndex;
			}
		}
	}
}

void level_info::PositionTiles(tile_map *_TileMap, tile_manager *TileManager)
{
	int DeathIndex = 0;
	int MoveIndex = 0;
	for (int Row = 0;
		Row < _TileMap->CountY;
		Row++)
	{
		for (int Column = 0;
			Column < _TileMap->CountX;
			Column++)
		{
			int TileID = _TileMap->Tiles[Row * _TileMap->CountX + Column];

			if (TileID == DEATH_TILE_ID)
			{
				TileManager->DeathTiles[DeathIndex].Initialize({ (float)Row, (float)Column },
					_TileMap->TileWidth, _TileMap->TileHeight,
					_TileMap->CountX, _TileMap->CountY,
					DEATH_TILE_ID);
				++DeathIndex;
			}
			else if (TileID == MOVE_TILE_ID_R)
			{
				TileManager->MoveTiles[MoveIndex].MoveInitialize({ (float)Row, (float)Column },
					_TileMap->TileWidth, _TileMap->TileHeight,
					_TileMap->CountX, _TileMap->CountY,
					MOVE_TILE_ID_R,
					{ 1, 0 });
				++MoveIndex;
			}
			else if (TileID == MOVE_TILE_ID_L)
			{
				TileManager->MoveTiles[MoveIndex].MoveInitialize({ (float)Row, (float)Column },
					_TileMap->TileWidth, _TileMap->TileHeight,
					_TileMap->CountX, _TileMap->CountY,
					MOVE_TILE_ID_L,
					{ -1, 0 });
				++MoveIndex;
			}
			else if (TileID == MOVE_TILE_ID_U)
			{
				TileManager->MoveTiles[MoveIndex].MoveInitialize({ (float)Row, (float)Column },
					_TileMap->TileWidth, _TileMap->TileHeight,
					_TileMap->CountX, _TileMap->CountY,
					MOVE_TILE_ID_U,
					{ 0, 1 });
				++MoveIndex;
			}
			else if (TileID == MOVE_TILE_ID_D)
			{
				TileManager->MoveTiles[MoveIndex].MoveInitialize({ (float)Row, (float)Column },
					_TileMap->TileWidth, _TileMap->TileHeight,
					_TileMap->CountX, _TileMap->CountY,
					MOVE_TILE_ID_D,
					{ 0, -1 });
				++MoveIndex;
			}
		}
	}
}

void level_info::Initialize(int _Row, int _Column,
							point InitDownLeftPos,
							float InitTileWidth, float InitTileHeight,
							int *PointerToTiles, object_manager *ObjectManager, tile_manager *TileManager)
{
	TileMap.Initialize(_Row, _Column, InitDownLeftPos, InitTileWidth, InitTileHeight, PointerToTiles);

	ObjectManager->NumChaser = 0;
	ObjectManager->NumTurret = 0;
	ObjectManager->NumSlow = 0;
	TileManager->NumDeath = 0;
	TileManager->NumMove = 0;
	
	for (int Row = 0;
		Row < TileMap.CountY;
		Row++)
	{
		for (int Column = 0;
			Column < TileMap.CountX;
			Column++)
		{
			int TileID = TileMap.Tiles[Row * TileMap.CountX + Column];

			//Check each tile IDs and do what should be done for each tiles. 
			if (TileID == PLAYER_TILE_ID)
			{
				InitPlayerPos.X = (float)Column;
				InitPlayerPos.Y = (float)Row;
			}
			else if(TileID == CHASER_TILE_ID)
			{
				++ObjectManager->NumChaser;
			}
			else if(TileID == TURRET_TILE_ID)
			{
				++ObjectManager->NumTurret;				
			}
			else if(TileID == SLOW_TILE_ID)
			{
				++ObjectManager->NumSlow;				
			}
			else if (TileID == OBSTACLE_CHASER_TILE_ID)
			{
				++ObjectManager->NumObstacleChaser;
			}
			else if (TileID == DEATH_TILE_ID)
			{
				++TileManager->NumDeath;
			}
			else if (TileID == MOVE_TILE_ID_R)
			{
				++TileManager->NumMove;
			}
			else if (TileID == MOVE_TILE_ID_L)
			{
				++TileManager->NumMove;
			}
			else if (TileID == MOVE_TILE_ID_U)
			{
				++TileManager->NumMove;
			}
			else if (TileID == MOVE_TILE_ID_D)
			{
				++TileManager->NumMove;
			}
		}
	}

	TileManager->DeathTiles = new death_tile[TileManager->NumDeath];
	TileManager->MoveTiles = new move_tile[TileManager->NumMove];
	ObjectManager->EnemyChasers = new enemy_chaser[ObjectManager->NumChaser];
	ObjectManager->EnemyTurrets = new enemy_turret[ObjectManager->NumTurret];
	ObjectManager->EnemySlow = new enemy_Slow[ObjectManager->NumSlow];	
	ObjectManager->ObstacleChasers = new obstacle_chaser[ObjectManager->NumObstacleChaser];

	PositionEnemies(&TileMap, ObjectManager);
	PositionTiles(&TileMap, TileManager);
}