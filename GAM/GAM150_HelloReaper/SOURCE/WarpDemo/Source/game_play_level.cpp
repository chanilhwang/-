/******************************************************************************
FileName: game_play_level.cpp
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
//Include Warp Engine
#include "WEInput.h"
#include "WEStateManager.h"
#include "WEApplication.h"
#include "WEGameData.h"
#include "WEMtx44.h"
#include "WEGraphics.h"
#include "WEVec2.h"
#include "WEMath.h"
#include "WERandom.h"
#include <stdio.h>
//For seed(time)
#include <ctime>
//Include own defined headers
#include "game_play_level.h"
#include "hreaper_intrinsics.h"
#include "player.h"
#include "level_info.h"
#include "enemy.h"
#include "object_manager.h"
#include "ui.h"
#include "draw_texture.h"
#include "particle.h"
#include "tilemaps.h"
#include "time_manager.h"
#include "camera.h"
#include "states_enum.h"
#include "obstacle.h"
#include "tile_Manager.h"
//Include fmod usage
#include "fmod_general.h"

namespace
{
	level_info CurrentLevel;
	player *Player;
	object_manager *ObjectManager;
	time_manager *TimeManager;
	tile_manager *TileManager;
	ui *UI;
	emitter *Emitters;
	camera Camera;

	color DeathColor;
	float DeathTime;


	//Level Clear Infos
	float LevelPlayTime;
	int DeathCount;

	/*Textures*/
	texture_info BlankTxt;
	//Player Texture
	int PlayerIdleTxt;
	int PlayerTxt;
	int PlayerDashIllusionTxt[7];
	int PlayerPistolTxt;
	int PlayerDashArrowTxt;
	int PlayerShadowTxt;
	int PlayerAttackTxt;
	int PlayerDeathTxt;
	int PlayerAttackEffectTxt;	
	int PlayerAttackEffectUpperTxt;

	//Game States
	bool IsPaused;
	bool IsStarted;	//Take a pause so that player can have some time
	//Map Textures
	texture_info FloorTxt;
	texture_info CeilingTxt;

	//Light Textures
	texture_info YellowLight;
	texture_info RedLight;

	//Enemy Textures
	int EnemyTurretTxt;
	int EnemyChaserTxt;
	int EnemySlowTxt;

	//Tile Textures
	int DeathTileTxt;
	int MoveTileLeftTxt;
	int MoveTileRightTxt;
	int MoveTileUpTxt;
	int MoveTileDownTxt;
	int TrapTileTxt;

	//Extra Textures
	int DeadTxt;
	int IsDeathEffectOn;
	int RestartTxt;
	int BackToShopTxt;
	int ClearTxt;
	int ContinueTxt;
	int PausedTxt;
	int BackToMainMenuTxt;
	int EndTxt;
	int EndingTxt;
	//Sounds
	fmod_sound_system PlayBGM;
	fmod_sound_system PlaySFX;

	//Pointer to the shared gamedata!
	WEGameData *GameData;
	
	// #define FONT_COUNT 26
	#define CurrentTileMap CurrentLevel.TileMap

	#define VISUAL_UP_MOVE_SPEED 0.03f
	#define VISUAL_DOWN_MOVE_SPEED 0.04f
	#define EACH_FRAME_TIME 0.05f

	//For the adjust of the player textures.
	#define PLAYER_OFFSET_Y 4.0f

	#define TOTAL_DEATH_TIME 3.0f

	#define MAX_GAME_LEVEL 7
}

void GamePlayingLoad(void)
{
	WERandom::Seed((int)time(0));

	PlayerIdleTxt= WEGraphics::LoadTexture("Textures\\Player\\player_idle.tga");
	PlayerTxt= WEGraphics::LoadTexture("Textures\\Player\\player_dash_right2.tga");
	PlayerAttackTxt = WEGraphics::LoadTexture("Textures\\Player\\player_attack.tga");
	PlayerDeathTxt = WEGraphics::LoadTexture("Textures\\Player\\player_death.tga");	
	PlayerAttackEffectTxt = WEGraphics::LoadTexture("Textures\\Player\\player_attack_effect.tga");
	PlayerAttackEffectUpperTxt = WEGraphics::LoadTexture("Textures\\Player\\player_attack_effect_upper.tga");	
	PlayerDashArrowTxt = WEGraphics::LoadTexture("Textures\\Player\\player_dash_arrow_effect.tga");
	PlayerDashIllusionTxt[0] = WEGraphics::LoadTexture("Textures\\Player\\player_dash_illusion_0.tga");
	PlayerDashIllusionTxt[1] = WEGraphics::LoadTexture("Textures\\Player\\player_dash_illusion_3.tga");
	PlayerDashIllusionTxt[2] = WEGraphics::LoadTexture("Textures\\Player\\player_dash_illusion_4.tga");
	PlayerDashIllusionTxt[3] = WEGraphics::LoadTexture("Textures\\Player\\player_dash_illusion_5.tga");
	PlayerDashIllusionTxt[4] = PlayerDashIllusionTxt[3];
	PlayerDashIllusionTxt[5] = PlayerDashIllusionTxt[3];
	PlayerDashIllusionTxt[6] = PlayerDashIllusionTxt[2];
	PlayerPistolTxt = WEGraphics::LoadTexture("Textures\\Player\\Guns\\player_pistol.tga");
	PlayerShadowTxt = WEGraphics::LoadTexture("Textures\\Player\\player_shadow.tga");

	BlankTxt.TextureID = WEGraphics::LoadTexture("Textures\\Blank.tga");
	FloorTxt.TextureID = WEGraphics::LoadTexture("Textures\\Maps\\Map0\\map0_floor.tga");	
	CeilingTxt.TextureID = WEGraphics::LoadTexture("Textures\\Maps\\Map0\\map0_ceiling.tga");
	YellowLight.TextureID = WEGraphics::LoadTexture("Textures\\Maps\\Map0\\light_yellow.tga");
	RedLight.TextureID = WEGraphics::LoadTexture("Textures\\Maps\\Map0\\light_bright_red.tga");
	DeadTxt = WEGraphics::LoadTexture("Textures\\dead.tga");
	RestartTxt = WEGraphics::LoadTexture("Textures\\restart.tga");
	BackToShopTxt = WEGraphics::LoadTexture("Textures\\back_to_shop.tga");
	BackToMainMenuTxt = WEGraphics::LoadTexture("Textures\\back_to_main_menu.tga");
	EnemyChaserTxt = WEGraphics::LoadTexture("Textures\\Enemy\\enemy_chaser.tga");
	EnemyTurretTxt = WEGraphics::LoadTexture("Textures\\Enemy\\enemy_turret.tga");
	EnemySlowTxt = WEGraphics::LoadTexture("Textures\\Enemy\\enemy_slow.tga");

	DeathTileTxt = WEGraphics::LoadTexture("Textures\\Tile\\DeathTileTxt.tga");
	MoveTileLeftTxt = WEGraphics::LoadTexture("Textures\\Tile\\MoveTileLeftTxt.tga");
	MoveTileRightTxt = WEGraphics::LoadTexture("Textures\\Tile\\MoveTileRightTxt.tga");
	MoveTileUpTxt = WEGraphics::LoadTexture("Textures\\Tile\\MoveTileUpTxt.tga");
	MoveTileDownTxt = WEGraphics::LoadTexture("Textures\\Tile\\MoveTileDownTxt.tga");
	TrapTileTxt = WEGraphics::LoadTexture("Textures\\Tile\\TrapTileTxt.tga");

	ClearTxt = WEGraphics::LoadTexture("Textures\\clear.tga");
	ContinueTxt = WEGraphics::LoadTexture("Textures\\continue.tga");
	PausedTxt = WEGraphics::LoadTexture("Textures\\paused.tga");
	EndTxt = WEGraphics::LoadTexture("Textures\\aend.tga");
	EndingTxt = WEGraphics::LoadTexture("Textures\\ending.tga");

	PlayBGM.MakeSound("Sounds\\BGM\\PreAlphaBattle.mp3", true);		
	PlayBGM.PlaySound();
}

void GamePlayingInit(void)
{
	ObjectManager = new object_manager;
	*ObjectManager = { 0 };
	TileManager = new tile_manager;
	*TileManager = {0};
	
	//Get the reserved game data
	GameData = WEStateManager::GetGameData();

	//Initialize level related variables to use at switch statement
	int TileCountX = 0;
	int TileCountY = 0;
	float TileWidth = 0.0f;
	float TileHeight = 0.0f;
	int *TileMapPointer = 0;

	//Tile Offset
	TileCountX = 7;
	TileCountY = 7;
	TileWidth = 15.8f;
	TileHeight = TileWidth * 0.8f;

	switch(GameData->CurrentLevelID)
	{
		//Tutorial
		case 0:
		{
			TileMapPointer = (int *)TutorialTilemap1;
		}break;

		case 1:
		{
			TileMapPointer = (int *)TutorialTilemap2;
		}break;
		
		case 2:
		{
			TileMapPointer = (int *)TutorialTilemap3;
		}break;

		case 3:
		{
			TileMapPointer = (int *)TutorialTilemap4;
		}break;

		case 4:
		{
			TileMapPointer = (int *)TutorialTilemap5;
		}break;

		case 5:
		{
			TileMapPointer = (int *)TutorialTilemap6;
		}break;

		case 6:
		{
			TileMapPointer = (int *)TutorialTilemap7;
		}break;

		case 7:
		{
			TileMapPointer = (int *)TutorialTilemap8;
		}break;

		default:
		{
			TileMapPointer = (int *)TutorialTilemap4;
		}break;
	}

	CurrentLevel.Initialize(TileCountX, TileCountY, //TileMapSize
						    {-1 * (TileCountX) * 0.5f * TileWidth, 
							-1 * (TileCountY) * 0.5f * TileHeight}, //DownLeftPos
						    TileWidth, TileHeight, //TileSize
							TileMapPointer, ObjectManager, TileManager);

	Player = new player(CurrentLevel.InitPlayerPos,
			   			CurrentTileMap.TileWidth, CurrentTileMap.TileHeight,
						CurrentTileMap.CountX, CurrentTileMap.CountY,
			   			CurrentTileMap.DownLeftPos, GameData);

	TimeManager = new time_manager(0.01f, 0.0f);

	Emitters = new emitter;
	Emitters->Initialize(40);

	UI = new ui;
	UI->Initialize(Player->Health, Player->Energy);
	//ObjectManager->CreateList();

	//Initialize Loaded Textures
	WEGraphics::SetBackgroundColor(0.098039f, 0.094117f, 0.113725f);
	// WEGraphics::SetBackgroundColor(0.203921f, 0.219607f, 0.254901f);

	// PlayBGM.PlaySound();
	FloorTxt.Pos = {-0.5f, 0.0f};
	
	//Light_red
	RedLight.Pos = {20.5f, 
						-13.5f};
	DeathColor = {255, 205, 205, 45.0f};
	DeathTime = 0.0f;
	IsDeathEffectOn = false;
	Camera.SetShake(0.0f, 0.0f);
	LevelPlayTime = 0.0f;

	IsStarted = false;
	IsPaused = false;
}

void GamePlayingUpdate(float dt)
{
	//Restart
	if((!WEInput::IsRepeating(WE_R) && WEInput::IsTriggered(WE_R)) || 
		(!WEInput::IsRepeating(WE_GAMEPAD_Y) && WEInput::IsTriggered(WE_GAMEPAD_Y)) && Player->State != -2)
	{
		Player->Initialize(CurrentLevel.InitPlayerPos,
			   				CurrentTileMap.TileWidth, CurrentTileMap.TileHeight,
							   CurrentTileMap.CountX, CurrentTileMap.CountY,
			   				CurrentTileMap.DownLeftPos, GameData);

		CurrentLevel.PositionEnemies(&CurrentLevel.TileMap, ObjectManager);
		CurrentLevel.PositionTiles(&CurrentLevel.TileMap, TileManager);

		DeathColor = {255, 205, 205, 45.0f};
		DeathTime = 0.0f;

		Camera.SetShake(0.0f, 0.0f);
		IsDeathEffectOn = false;

		IsPaused = false;
		IsStarted = true;
	}

	//Pause
	if ((!WEInput::IsRepeating(WE_P) && WEInput::IsTriggered(WE_P)) ||
		(!WEInput::IsRepeating(WE_ESCAPE) && WEInput::IsTriggered(WE_ESCAPE)) ||
		(!WEInput::IsRepeating(WE_GAMEPAD_START) && WEInput::IsTriggered(WE_GAMEPAD_START)) && Player->State >= 0)
	{
		if (IsPaused == true)
			IsPaused = false;
		else
			IsPaused = true;
	}

	//Is Paused, player can get back to main menu
	if (IsPaused)
	{
		if ((!WEInput::IsRepeating(WE_M) && WEInput::IsTriggered(WE_M)) || 
			(!WEInput::IsRepeating(WE_GAMEPAD_B) && WEInput::IsTriggered(WE_GAMEPAD_B)))
		{
			WEStateManager::SetNextState(STATE_MAIN_MENU);
		}
	}

	/*God Mode*/
	if(!WEInput::IsRepeating(WE_G) && WEInput::IsTriggered(WE_G))
	{
		if (Player->GodMode == false)
			Player->GodMode = true;
		else
			Player->GodMode = false;
		if(Player->Invincible == true)
		{
			Player->Invincible = false;
		}
		else
			Player->Invincible = true;

		if(Player->IsInfiniteDashOn == true)
			Player->IsInfiniteDashOn = false;
		else
			Player->IsInfiniteDashOn = true;			
	}

	if(Player->IsInfiniteDashOn)
		Player->Energy = Player->MaxEnergy;

	if(!WEInput::IsRepeating(WE_Q) && WEInput::IsTriggered(WE_Q))
	{
		TimeManager->SlowDownLength = 0.0f;
		IsStarted = true;
		IsPaused = false;
		ObjectManager->KillAllEnemies();
	}

	//Give the player short time to plan and think
	if (!IsStarted)
	{
		if ((!WEInput::IsRepeating(WE_SPACE) && WEInput::IsTriggered(WE_SPACE)) || 
			(!WEInput::IsRepeating(WE_GAMEPAD_A) && WEInput::IsTriggered(WE_GAMEPAD_A)))
		{
			IsStarted = true;
		}
	}

	/*Manage Time(dt)*/
	if (TimeManager->CheckRemainingTime())
	{
		TimeManager->SlowDownLength -= dt;
		dt = TimeManager->DoSlowmotion(dt);
	}

	//If paused, pause the gmae
	if (IsPaused || !IsStarted)
	{
		dt = 0.0f;
	}

	//Back to the Main Menu
	if (!WEInput::IsRepeating(WE_M) && WEInput::IsTriggered(WE_M))
	{
		WEStateManager::SetNextState(STATE_MAIN_MENU);
	}

	//Process Player
	if(!IsPaused && IsStarted)
	Player->PlayerProcess(dt, &CurrentTileMap, *ObjectManager, *TimeManager, Camera, GameData->SFXVolume);

	//(g.lee)Based on player position, the camera(sort of) should be moved.
	//For example, without this, enemy will be moving if the player moved.
	//That's not we want, so we should had this offset to the enemy drawing position
	//so that enemy is not relevant to the player.
	point Offset = {((CurrentTileMap.CountX) * 0.5f * CurrentTileMap.TileWidth - (Player->Pos.X - CurrentTileMap.DownLeftPos.X)) * VISUAL_DOWN_MOVE_SPEED,
					((CurrentTileMap.CountY) * 0.5f * CurrentTileMap.TileHeight - (Player->Pos.Y - CurrentTileMap.DownLeftPos.Y)) * VISUAL_DOWN_MOVE_SPEED};

	//Process Chaser
	for(int Index = 0; Index < ObjectManager->NumChaser; Index++)
		ObjectManager->EnemyChasers[Index].EnemyChaserProcess(Player, dt, &CurrentTileMap, GameData->SFXVolume);

	//Process Turret
	for (int Index = 0; Index < ObjectManager->NumTurret; Index++)
		ObjectManager->EnemyTurrets[Index].EnemyTurretProcess(Player, dt);

	//Process Slow
	for (int Index = 0; Index < ObjectManager->NumSlow; Index++)
		ObjectManager->EnemySlow[Index].EnemySlowProcess(Player, dt, &CurrentTileMap);
	
	for (int Index = 0; Index < ObjectManager->NumObstacleChaser; Index++)
		ObjectManager->ObstacleChasers[Index].ObstacleChaserProcess(Player, dt);

	//Process DeathTile
	for (int Index = 0; Index < TileManager->NumDeath; Index++)
		TileManager->DeathTiles[Index].DeathTileProcess(Player);
	//Process MoveTile
	for (int Index = 0; Index < TileManager->NumMove; Index++)
		TileManager->MoveTiles[Index].MoveForPlayer(Player, dt);

	//Process Textures based on the player
	//Ceiling
	CeilingTxt.Pos = {Offset.X * 1.4f, 
						Offset.Y * 1.4f};
	//Light_yellow
	YellowLight.Pos = {5.5f + Offset.X * 1.4f, 
						30.5f + Offset.Y * 1.4f};

	//Update Particles
	Emitters->UpdateParticles(dt);

	//Update UI
	UI->UpdateUI(Player);
	


	Camera.Update({-Offset.X, -Offset.Y}, dt);
	WEGraphics::SetCamera(Camera.Pos.X, Camera.Pos.Y, 70.0f, 0.0f);

	WEGraphics::SetToPerspective();

	//Start Drawing
	WEGraphics::StartDraw();
	WEMtx44 transform;

	//Draw Floor
	WEGraphics::SetTextureColor(0xffffffff);
    WEGraphics::SetTexture(FloorTxt.TextureID);
    WEMtx44::MakeTransform(transform, 111.0f, 111.0f,
                            0.0f,
                            FloorTxt.Pos.X, FloorTxt.Pos.Y,
                            0.0f);
	WEGraphics::Draw(transform);

	TileManager->DrawTiles(transform,DeathTileTxt,
		MoveTileLeftTxt, MoveTileRightTxt,
		MoveTileUpTxt, MoveTileDownTxt,
		Offset);

	
	/*Draw Player!!*/

	//Player Shadow
	WEGraphics::SetTextureColor(0xffffffff);
    WEGraphics::SetTexture(PlayerShadowTxt);
    WEMtx44::MakeTransform(transform, Player->DirX * -10.0f, 10.0f,
                            0.0f,
							Player->Pos.X - Player->DirX * 0.5f, Player->Pos.Y - 1.4f,
                            0.0f);
	WEGraphics::Draw(transform);

	if(Player->AttackAnimationOn && Player->AttackingTime >= 0.0f)
	{

		WEGraphics::SetTextureColor((unsigned char)(253),
									(unsigned char)(32), 
									(unsigned char)(154), 
									(unsigned char)(150));
		if(Player->AttackDirY == 0)
		{
			WEGraphics::SetTexture(PlayerAttackTxt);
			WEGraphics::SetTextureCoords(0.125f, 1.0f, 0.0f, Player->AttackTextCoord, 0);
			WEMtx44::MakeTransform(transform, Player->AttackDirX * 30.0f, 15.0f,
								0.0f,
								Player->AttackPos.X - Player->AttackDirX * 8.0f, Player->AttackPos.Y + PLAYER_OFFSET_Y,
								0.0f);

			WEGraphics::Draw(transform);
			
			WEGraphics::SetTextureColor((unsigned char)(101),
										(unsigned char)(255), 
										(unsigned char)(184), 
										(unsigned char)(255));
			WEGraphics::SetTexture(PlayerAttackEffectTxt);
			WEGraphics::SetTextureCoords(0.125f, 1.0f, 0.0f, Player->AttackTextCoord, 0);
			WEMtx44::MakeTransform(transform, Player->AttackDirX * 40.0f, 20.0f,
								Player->AttackDirY * 90.0f,
								Player->AttackPos.X + Player->AttackDirX * 15.0f, Player->AttackPos.Y + 1.1f * PLAYER_OFFSET_Y,
								0.0f);

			WEGraphics::Draw(transform);
		}
		else
		{
			WEGraphics::SetTextureColor((unsigned char)(101),
										(unsigned char)(255), 
										(unsigned char)(184), 
										(unsigned char)(255));
			WEGraphics::SetTexture(PlayerAttackEffectUpperTxt);
			WEGraphics::SetTextureCoords(0.125f, 1.0f, 0.0f, Player->AttackTextCoord, 0);
			WEMtx44::MakeTransform(transform, 20.0f, Player->AttackDirY * 40.0f,
								0.0f,
								Player->AttackPos.X, Player->AttackPos.Y + Player->AttackDirY * 18.0f,
								0.0f);

			WEGraphics::Draw(transform);
		}
	}
	//Draw Player Dash Illusion, if there's any.

	if(Player->DashType == 1)
	{
		WEGraphics::SetTextureCoords(1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		for(int i = 0;
			i < Player->PlayerIllusionCount;
			++i)
		{
			WEGraphics::SetTextureColor((unsigned char)(60 + i * 5),
										(unsigned char)(150 + i * 5), 
										(unsigned char)(110 + i * 5), 
										(unsigned char)(180 + i * 5));
			WEGraphics::SetTexture(PlayerDashIllusionTxt[i]);
			WEMtx44::MakeTransform(transform, Player->DashDirX * 30.0f , 15.0f,
									0.0f,
									Player->PlayerIllusion[i].X - Player->DashDirY * Player->DashDirX * 3.5f,
									Player->PlayerIllusion[i].Y + PLAYER_OFFSET_Y,
									0.0f);
									
			WEGraphics::Draw(transform);		
		}
	}
	
	//player animation
	switch(Player->State)
	{
		case -2:
		case -1:
		{
			WEGraphics::SetTexture(PlayerDeathTxt);
			WEGraphics::SetTextureCoords(0.125f, 1.0f, 0.0f, Player->TextCoord, 0);
		}break;
		case 0:
		{
			WEGraphics::SetTexture(PlayerIdleTxt);
			WEGraphics::SetTextureCoords(0.125f, 1.0f, 0.0f, Player->TextCoord, 0);
			if(Player->PlayerIllusionCount > 0)
				Player->PlayerIllusionCount--;
		}break;
		case 1:
		{
			WEGraphics::SetTexture(PlayerTxt);
			WEGraphics::SetTextureCoords(.125f, 1.0f, 0.0f, Player->TextCoord, 0.0f);
			
		}break;
	}
	WEGraphics::SetTextureColor(0xffffffff);
	
	WEMtx44::MakeTransform(transform, Player->DirX * 30.0f, 15.0f,
							0.0f,
							Player->Pos.X,
							Player->Pos.Y + PLAYER_OFFSET_Y,
							0.0f);
	
	WEGraphics::Draw(transform);
	
	if(Player->Invincible)
	{	
		WEGraphics::SetTextureColor((unsigned char)(101),
									(unsigned char)(255), 
									(unsigned char)(184), 
									(unsigned char)(150));
		WEGraphics::SetTexture(BlankTxt.TextureID);

		WEMtx44::MakeTransform(transform, Player->Height, Player->Height,
								0.0f,
								Player->Pos.X,
								Player->Pos.Y + PLAYER_OFFSET_Y,
								0.0f);

		WEGraphics::Draw(transform);					
	}
	//Draw PlayerDebug Rect
	// WEGraphics::SetTexture(BlankTxt.TextureID);	
	// WEMtx44::MakeTransform(transform, Player->Width, Player->Height,
	// 						0.0f,
	// 						Player->Pos.X,
	// 						Player->Pos.Y,
	// 						0.0f);
	
	// WEGraphics::Draw(transform);

	//Draw Player Pistol
	WEGraphics::SetTexture(PlayerPistolTxt);
	
	WEMtx44::MakeTransform(transform, 8, 8,
					0.0f,
					Player->GunPos.X,
					Player->GunPos.Y + PLAYER_OFFSET_Y,
					0.0f);
	WEGraphics::SetTextureCoords(.125f, 1.0f, 0.0f, Player->TextCoord, 0);
	WEGraphics::Draw(transform);

	WEGraphics::SetTextureCoords(1.0f, 1.0f, 0.0f, 0.0f, 0.0f);

	//Draw other objects
	ObjectManager->DrawObjects(transform, 
		BlankTxt.TextureID, EnemyChaserTxt, EnemyTurretTxt, EnemySlowTxt, 
		CurrentTileMap.CountX * CurrentTileMap.TileWidth, Offset);

	//Draw lights
	//light_yellow
	WEGraphics::SetTextureColor(0xffffffff);
    WEGraphics::SetTexture(YellowLight.TextureID);
    WEMtx44::MakeTransform(transform, 60.0f, 60.0f,
                            0.0f,
                            YellowLight.Pos.X, YellowLight.Pos.Y,
                            0.0f);

    WEGraphics::Draw(transform);  

	//light_red
	WEGraphics::SetTextureColor(0xffffffff);
    WEGraphics::SetTexture(RedLight.TextureID);
    WEMtx44::MakeTransform(transform, 30.0f, 30.0f,
                            0.0f,
                            RedLight.Pos.X, RedLight.Pos.Y,
                            0.0f);

    WEGraphics::Draw(transform);  

	//Draw Particles
	for (int Index = 0; 
		 Index < Emitters->ParticleNum; 
		 ++Index)
    {
		particle* Particle = &Emitters->Particles[Index];

		if(Particle)
		{
			WEGraphics::SetTextureColor((unsigned char)Particle->Color.Red, 
										(unsigned char)Particle->Color.Green, 
										(unsigned char)Particle->Color.Blue, 
										RoundFloatToUChar(Particle->Color.Alpha));
			WEGraphics::SetTexture(BlankTxt.TextureID);
			WEMtx44::MakeTransform(transform, Particle->Scale.X, Particle->Scale.Y,
									0.0f,
									Particle->Pos.X, Particle->Pos.Y,
									0.0f);
			//Draw Object
			WEGraphics::Draw(transform);
		}
    }

	//Draw ceiling shadow
    WEGraphics::SetTextureColor(0x99ffffff);
    WEGraphics::SetTexture(CeilingTxt.TextureID);
    WEMtx44::MakeTransform(transform, 120.0f, 120.0f,
                            0.0f,
                            FloorTxt.Pos.X, FloorTxt.Pos.Y - 12.0f,
                            0.0f);

    WEGraphics::Draw(transform);  

	//Draw ceiling
    WEGraphics::SetTextureColor(0xffffffff);
    WEGraphics::SetTexture(CeilingTxt.TextureID);
    WEMtx44::MakeTransform(transform, 150.0f, 150.0f,
                            0.0f,
                            CeilingTxt.Pos.X, CeilingTxt.Pos.Y,
                            0.0f);

    WEGraphics::Draw(transform);  

	WEGraphics::SetTextureColor((unsigned char)(25),
								(unsigned char)(24),
								(unsigned char)(29),
								(unsigned char)(255));
    WEGraphics::SetTexture(BlankTxt.TextureID);
    WEMtx44::MakeTransform(transform, 40.0f, 15.0f,
                            0.0f,
                            CeilingTxt.Pos.X, CeilingTxt.Pos.Y + 60.0f,
                            0.0f);

    WEGraphics::Draw(transform);  


	if(Player->State <= -1)
	{
		if(!IsDeathEffectOn)
		{
			PlaySFX.MakeSound("Sounds\\SFX\\death_sound.mp3", false);			
			PlaySFX.PlaySound();
			IsDeathEffectOn = true;
		}
		if(DeathTime <= 0.5f * TOTAL_DEATH_TIME)
		{
			DeathColor.Red -= 35 * DeathTime;
			DeathColor.Green -= 35 * DeathTime;
			DeathColor.Blue -= 35 * DeathTime;
			DeathColor.Alpha += 50 * DeathTime;
			if(DeathColor.Red <= 189)
				DeathColor.Red  = 189;
			if(DeathColor.Green <= 36)
				DeathColor.Green  = 36;
			if(DeathColor.Blue <= 36)
				DeathColor.Blue  = 36;
			if(DeathColor.Alpha >= 255.0f)
				DeathColor.Alpha = 250.0f;
			WEGraphics::SetTextureColor(DeathColor.Red,
										DeathColor.Green,
										DeathColor.Blue, 
										(unsigned char)DeathColor.Alpha);
			WEGraphics::SetTexture(BlankTxt.TextureID);	
			WEMtx44::MakeTransform(transform, 500, 400,
								0.0f,
								0.0f,
								0.0f,
								0.0f);
			WEGraphics::Draw(transform); 										
			Camera.SetShake(0.8f * 1.1f, 6.0f * DeathTime);
		}
		else
		{
			WEGraphics::SetTextureColor((unsigned char)(25),
										(unsigned char)(24),
										(unsigned char)(29),
										(unsigned char)(255));
			WEGraphics::SetTexture(BlankTxt.TextureID);	
			WEMtx44::MakeTransform(transform, 500, 400,
									0.0f,
									0.0f,
									0.0f,
									0.0f);
			WEGraphics::Draw(transform); 
			WEGraphics::SetTextureColor((unsigned char)(255),
										(unsigned char)(255),
										(unsigned char)(255),
										(unsigned char)(255));
			WEGraphics::SetTexture(DeadTxt);	
			WEMtx44::MakeTransform(transform, 22, 22,
									0.0f,
									0.0f,
									0.0f,
									0.0f);
			WEGraphics::Draw(transform);

			WEGraphics::SetTexture(RestartTxt);	
			WEMtx44::MakeTransform(transform, 32, 8,
									0.0f,
									0.0f,
									-30.0f,
									0.0f);
			WEGraphics::Draw(transform);

		}
		DeathTime += dt;
	}
	else
	{
		//Draw UI		
		UI->DrawUI(BlankTxt.TextureID);			
	}

	//Win Condition
	if(ObjectManager->DidEnemyDied() && Player->State != -1)
	{
		Player->State = -2;
		if(GameData->CurrentLevelID >= MAX_GAME_LEVEL)
		{
			if(DeathTime <= 0.5f * TOTAL_DEATH_TIME)
			{
				DeathColor.Red -= 35 * DeathTime;
				DeathColor.Green -= 35 * DeathTime;
				DeathColor.Blue -= 35 * DeathTime;
				DeathColor.Alpha += 50 * DeathTime;
				if(DeathColor.Red <= 189)
					DeathColor.Red  = 189;
				if(DeathColor.Green <= 36)
					DeathColor.Green  = 36;
				if(DeathColor.Blue <= 36)
					DeathColor.Blue  = 36;
				if(DeathColor.Alpha >= 255.0f)
					DeathColor.Alpha = 252.0f;
				WEGraphics::SetTextureColor(DeathColor.Red,
											DeathColor.Green,
											DeathColor.Blue, 
											(unsigned char)DeathColor.Alpha);
				WEGraphics::SetTexture(BlankTxt.TextureID);	
				WEMtx44::MakeTransform(transform, 500, 400,
									0.0f,
									0.0f,
									0.0f,
									0.0f);
				Camera.SetShake(0.8f * 0.27f, 4.0f * DeathTime);
			}
			else
			{
				WEGraphics::SetTextureColor((unsigned char)(25),
											(unsigned char)(24),
											(unsigned char)(29),
											(unsigned char)(255));
				WEGraphics::SetTexture(BlankTxt.TextureID);	
				WEMtx44::MakeTransform(transform, 500, 400,
										0.0f,
										0.0f,
										0.0f,
										0.0f);
				WEGraphics::Draw(transform);

				WEGraphics::SetTextureColor((unsigned char)(255),
											(unsigned char)(255),
											(unsigned char)(255),
											(unsigned char)(255));

				WEGraphics::SetTextureColor(0xffffffff);
				WEGraphics::SetTexture(EndingTxt);
				WEMtx44::MakeTransform(transform, 70.0f, 70.0f,
										0.0f,
										0.0f, 5.0f,
										0.0f);

				WEGraphics::Draw(transform);

				WEGraphics::SetTexture(EndTxt);	
				WEMtx44::MakeTransform(transform, 40, 10,
										0.0f,
										5.0f,
										-40.0f,
										0.0f);
				WEGraphics::Draw(transform);

				if ((!WEInput::IsRepeating(WE_SPACE) && WEInput::IsTriggered(WE_SPACE)) || 
					(!WEInput::IsRepeating(WE_GAMEPAD_A) && WEInput::IsTriggered(WE_GAMEPAD_A)))
				{

					WEGraphics::EndDraw();
					GameData->PlayTime += LevelPlayTime;
					WEStateManager::SetNextState(STATE_MAIN_MENU);					
				}
			}
			DeathTime += dt;
		}
		else// if the game has not reached max level
		{
			if(DeathTime <= 0.5f * TOTAL_DEATH_TIME)
			{
				DeathColor.Red -= 35 * DeathTime;
				DeathColor.Green -= 35 * DeathTime;
				DeathColor.Blue -= 35 * DeathTime;
				DeathColor.Alpha += 50 * DeathTime;
				if(DeathColor.Red <= 189)
					DeathColor.Red  = 189;
				if(DeathColor.Green <= 36)
					DeathColor.Green  = 36;
				if(DeathColor.Blue <= 36)
					DeathColor.Blue  = 36;
				if(DeathColor.Alpha >= 255.0f)
					DeathColor.Alpha = 250.0f;
				WEGraphics::SetTextureColor(DeathColor.Red,
											DeathColor.Green,
											DeathColor.Blue, 
											(unsigned char)DeathColor.Alpha);
				WEGraphics::SetTexture(BlankTxt.TextureID);	
				WEMtx44::MakeTransform(transform, 500, 400,
									0.0f,
									0.0f,
									0.0f,
									0.0f);
				Camera.SetShake(0.8f * 0.27f, 4.0f * DeathTime);
			}
			else
			{
				WEGraphics::SetTextureColor((unsigned char)(25),
                                       (unsigned char)(24),
                                       (unsigned char)(29),
                                       (unsigned char)(255));
				WEGraphics::SetTexture(BlankTxt.TextureID);	
				WEMtx44::MakeTransform(transform, 500, 400,
										0.0f,
										0.0f,
										0.0f,
										0.0f);
				WEGraphics::Draw(transform);

				WEGraphics::SetTextureColor((unsigned char)(255),
                                       (unsigned char)(255),
                                       (unsigned char)(255),
                                       (unsigned char)(255));
				WEGraphics::SetTexture(ClearTxt);	
				WEMtx44::MakeTransform(transform, 22, 22,
										0.0f,
										5.0f,
										0.0f,
										0.0f);
				WEGraphics::Draw(transform);

				char Text[30];
				sprintf_s(Text, "Clear Time : %.1f", LevelPlayTime);
				WEGraphics::SetTextureColor(0xFFFFFFff);
				WEGraphics::WriteText(Text, -18.0f, -20.0f);

				WEGraphics::SetTexture(ContinueTxt);	
				WEMtx44::MakeTransform(transform, 64, 8,
										0.0f,
										4.5f,
										-30.0f,
										0.0f);
				WEGraphics::Draw(transform);
			}
			DeathTime += dt;
		}
	}

	//If paused, draw something else
	if(IsPaused)
	{
		WEVec2 TopLeft;
		WEVec2 BottomRight;
		WEGraphics::GetWorldTopLeft(TopLeft);
		WEGraphics::GetWorldBotRight(BottomRight);

		WEGraphics::SetTexture(BlankTxt.TextureID);	
		WEGraphics::SetTextureColor(0x55000000);
		WEMtx44::MakeTransform(transform, BottomRight.x - TopLeft.x, TopLeft.y - BottomRight.y,
								0.0f,
								0.0f,
								0.0f,
								0.0f);
		WEGraphics::Draw(transform);

		WEGraphics::SetTexture(PausedTxt);	
		WEGraphics::SetTextureColor(0xffffffff);
		WEMtx44::MakeTransform(transform, 60.0f, 15.0f,
								0.0f,
								0.0f,
								0.0f,
								0.0f);
		WEGraphics::Draw(transform);

		WEGraphics::SetTexture(BackToMainMenuTxt);	
		WEGraphics::SetTextureColor(0xffffffff);
		WEMtx44::MakeTransform(transform, 40.0f, 10.0f,
								0.0f,
								0.0f,
								-15.0f,
								0.0f);
		WEGraphics::Draw(transform);
	}

	WEGraphics::EndDraw();
	
	PlayBGM.channel->setVolume(0.005f * GameData->BGMVolume);
	PlaySFX.channel->setVolume(0.005f * GameData->SFXVolume);
	PlayBGM.system->update();
	PlaySFX.system->update();

	if(Player->State > -1)
		LevelPlayTime += dt;

	if(Player->State < -1 && GameData->CurrentLevelID < MAX_GAME_LEVEL)
	{
		if ((!WEInput::IsRepeating(WE_SPACE) && WEInput::IsTriggered(WE_SPACE)) || 
		(!WEInput::IsRepeating(WE_GAMEPAD_A) && WEInput::IsTriggered(WE_GAMEPAD_A)))
		{
			GameData->PlayTime += LevelPlayTime;
			WEStateManager::SetNextState(STATE_UPGRADE_LEVEL);
		}
	}
}

void GamePlayingShutdown(void)
{
	Player->PlayerSFX.system->close();
	Player->PlayerSFX.system->release();	

	PlayBGM.system->close();
	PlayBGM.system->release();

	PlaySFX.system->close();
	PlaySFX.system->release();

	for(int i = 0;
		i < ObjectManager->NumChaser;
		++i)
	{
		ObjectManager->EnemyChasers[i].EnemySFX.system->close();
		ObjectManager->EnemyChasers[i].EnemySFX.system->release();		
	}

	delete Player;
	ObjectManager->DestroyObjects();
	delete ObjectManager;
	TileManager->DestroyTiles();
	delete TileManager;
	Emitters->DeleteParticles();
	delete Emitters;
	delete TimeManager;
	delete UI;
}

void GamePlayingUnload(void)
{
	WEGraphics::UnloadTexture(BlankTxt.TextureID);

	WEGraphics::UnloadTexture(PlayerIdleTxt);
	WEGraphics::UnloadTexture(PlayerTxt);
	WEGraphics::UnloadTexture(PlayerDashArrowTxt);
	WEGraphics::UnloadTexture(PlayerPistolTxt);
	WEGraphics::UnloadTexture(PlayerAttackTxt);
	WEGraphics::UnloadTexture(PlayerDeathTxt);	
	WEGraphics::UnloadTexture(PlayerAttackEffectTxt);	
	WEGraphics::UnloadTexture(PlayerAttackEffectUpperTxt);		
	WEGraphics::UnloadTexture(PlayerDashIllusionTxt[0]);
	WEGraphics::UnloadTexture(PlayerDashIllusionTxt[1]);
	WEGraphics::UnloadTexture(PlayerDashIllusionTxt[2]);
	WEGraphics::UnloadTexture(PlayerDashIllusionTxt[3]);
	WEGraphics::UnloadTexture(PlayerDashIllusionTxt[4]);
	WEGraphics::UnloadTexture(PlayerShadowTxt);

	WEGraphics::UnloadTexture(FloorTxt.TextureID);
	WEGraphics::UnloadTexture(CeilingTxt.TextureID);
	
	WEGraphics::UnloadTexture(YellowLight.TextureID);
	WEGraphics::UnloadTexture(RedLight.TextureID);

	WEGraphics::UnloadTexture(DeadTxt);
	WEGraphics::UnloadTexture(RestartTxt);	
	WEGraphics::UnloadTexture(BackToShopTxt);		
	WEGraphics::UnloadTexture(EndTxt);			

	WEGraphics::UnloadTexture(EnemyChaserTxt);	
	WEGraphics::UnloadTexture(EnemyTurretTxt);
	WEGraphics::UnloadTexture(EnemySlowTxt);	
	
	WEGraphics::UnloadTexture(DeathTileTxt);
	WEGraphics::UnloadTexture(MoveTileLeftTxt); 
	WEGraphics::UnloadTexture(MoveTileRightTxt);
	WEGraphics::UnloadTexture(MoveTileUpTxt);
	WEGraphics::UnloadTexture(MoveTileDownTxt);
	WEGraphics::UnloadTexture(TrapTileTxt);
	WEGraphics::UnloadTexture(ClearTxt);
	WEGraphics::UnloadTexture(ContinueTxt);		
	WEGraphics::UnloadTexture(PausedTxt);
	WEGraphics::UnloadTexture(BackToMainMenuTxt);
	WEGraphics::UnloadTexture(EndingTxt);
	
}