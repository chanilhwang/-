/******************************************************************************
FileName: player.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef PLAYER_H

#include "hreaper_general.h"
#include "tile_info.h"
#include "object_manager.h"
#include "time_manager.h"
#include "camera.h"
#include "WEGameData.h"

#include "fmod_general.h"

class player : public object
{
public:
	float MovingTime;
	float AttackingTime;
	int PlayerTileX;
	int PlayerTileY;
	float AttackingRange;

	int State;
	int DirX; //1 = Right, -1 = Left 
	int DirY; //1 = UP, -1 = Down

	float TotalTime;
	float TextCoord;
	point Start;
	point Destination;//Destination of the player when dashing.
					  //Used to move fixed amount of distance!

	point GunPos;
	
	//To not change the direction while doing other actions	
	int DashType;
	int DashDirX;
	int DashDirY;
	point PlayerIllusion[6];
	int PlayerIllusionCount;
	int Energy;
	int MaxEnergy;
	float DashUpgrade;
	
	point AttackPos;
	float TotalAttackTime;
	float AttackTextCoord;
	bool AttackAnimationOn;
	int AttackDirX;
	int AttackDirY;
	
	bool GodMode;
	bool IsInfiniteDashOn;
	fmod_sound_system PlayerSFX;

	player(point InitTilePos, 
		   float InputTileWidth, float InputTileHeight,
		   int _CountX, int _CountY,
		   point DownLeftPos, WEGameData *);
		   
	void Initialize(point InitTilePos, 
					float InputTileWidth, float InputTileHeight,
					int _CountX, int _CountY,
					point DownLeftPos, WEGameData *);

	void PlayerMovingInputProcess(float dt, tile_map *CurrentLevel);
	void PlayerAttackingInputProcess(float dt, tile_map *CurrentLevel, object_manager &ObjectManager, time_manager &, camera &, float SFXVolume);
	void PlayerInputProcess(float dt, tile_map *CurrentLevel, object_manager &ObjectManager, time_manager &, camera &, float SFXVolume);
	void PlayerProcess(float dt, tile_map *CurrentLevel, object_manager &ObjectManager, time_manager &, camera &, float SFXVolume);
};

#define PLAYER_H
#endif //PLAYER_H