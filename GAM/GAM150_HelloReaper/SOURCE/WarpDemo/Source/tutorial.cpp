/******************************************************************************
FileName: tutorial.cpp
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "tutorial.h"

#include "WEInput.h"
#include "WEStateManager.h"
#include "WEApplication.h"
#include "WEGameData.h"
#include "WEMtx44.h"
#include "WEGraphics.h"
#include "WEVec2.h"

#include <ctime>
#include "hreaper_intrinsics.h"
#include "player.h"
#include "level_info.h"
#include "enemy.h"
#include "object_manager.h"
#include "ui.h"
#include "draw_texture.h"
#include "particle.h"
#include "time_manager.h"
#include "camera.h"
#include "states_enum.h"

#include "fmod_general.h"
#include <cstdio>

namespace
{
	level_info TutorialLevel;
	player * Player;
	object_manager *ObjectManager;
	time_manager *TimeManager;
	tile_manager *TileManager;
	ui *UI;
	emitter *Emitters;
	camera Camera;

	//for tutorial
	checkingList CheckingList;

	texture_info BlankTxt;

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
	
	//Enemy Textures(For object manager)
	int EnemyTurretTxt;
	int EnemyChaserTxt;

	texture_info FloorTxt;
	texture_info CeilingTxt;

	texture_info YellowLight;
	texture_info RedLight;

	fmod_sound_system PlayBGM;
	fmod_sound_system PlaySFX;

	WEGameData *GameData;

	int CurrentTilemap[7][7]
		=
	{
		{ 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 7, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1 }
	};

	#define TutorialTileMap TutorialLevel.TileMap
	#define VISUAL_UP_MOVE_SPEED 0.03f
	#define VISUAL_DOWN_MOVE_SPEED 0.04f
	#define PLAYER_OFFSET_Y 4.0f
}

void tutorialLoad()
{
	PlayerIdleTxt = WEGraphics::LoadTexture("Textures\\Player\\player_idle.tga");
	PlayerTxt = WEGraphics::LoadTexture("Textures\\Player\\player_dash_right2.tga");
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

}
void tutorialInit()
{
	ObjectManager = new object_manager;
	*ObjectManager = { 0 };
	TileManager = new tile_manager;
	*TileManager = { 0 };

	GameData = WEStateManager::GetGameData();

	int TileCountX = 7;
	int TileCountY = 7;
	float TileWidth = 15.8f;
	float TileHeight = TileWidth * 0.8f;
	int *TileMapPointer = (int*)CurrentTilemap;

	PlayBGM.system->close();
	PlayBGM.system->release();

	TutorialLevel.Initialize(TileCountX, TileCountY, //TileMapSize
	{ -1 * (TileCountX) * 0.5f * TileWidth,
		-1 * (TileCountY) * 0.5f * TileHeight }, //DownLeftPos
		TileWidth, TileHeight, //TileSize
		TileMapPointer, ObjectManager, TileManager);

	Player = new player(TutorialLevel.InitPlayerPos,
		TutorialTileMap.TileWidth, TutorialTileMap.TileHeight,
		TutorialTileMap.CountX, TutorialTileMap.CountY,
		TutorialTileMap.DownLeftPos, GameData);

	TimeManager = new time_manager(0.01f, 0.0f);

	Emitters = new emitter;
	Emitters->Initialize(40);

	UI = new ui;
	UI->Initialize(Player->Health, Player->Energy);

	WEGraphics::SetBackgroundColor(0.098039f, 0.094117f, 0.113725f);

	FloorTxt.Pos = { -0.5f, 0.0f };
	RedLight.Pos = { 20.5f, -13.5f };
	Camera.SetShake(0.0f, 0.0f);

	PlayBGM.MakeSound("Sounds\\BGM\\PreAlphaBattle.mp3", true);
	// PlayBGM.PlaySound();
}

void tutorialUpdate(float dt)
{
	if(!WEInput::IsRepeating(WE_ESCAPE) && WEInput::IsTriggered(WE_ESCAPE))
	{
		WEStateManager::SetNextState(STATE_MAIN_MENU);
	}

	//For Tutorial
	if (CheckingList.Move1 > 0 &&
		Player->State == 1 &&
		((WEInput::GetLeftTrigger() >= 0.35f) ||
		(WEInput::IsPressed(WE_SHIFT))) &&
		!CheckingList.Lock)
	{
		CheckingList.Move1--;
		CheckingList.Lock = true;
	}

	if (CheckingList.Move2 > 0 &&
		Player->State == 1 &&
		(WEInput::GetLeftTrigger() < 0.35f) &&
		!CheckingList.Lock)
	{
		CheckingList.Move2--;
		CheckingList.Lock = true;
	}

	if (CheckingList.Attacking > 0 &&
		Player->AttackingTime > 0.0f &&
		!CheckingList.Lock)
	{
		CheckingList.Attacking--;
		CheckingList.Lock = true;
	}

	if (Player->State == 0 &&
		Player->AttackingTime < 0.0f &&
		CheckingList.Lock)
	{
		CheckingList.Lock = false;
	}

	if (Player->Energy == 0)
	{
		Player->Energy = 5;
		CheckingList.IsEnergyExhausted = true;
		CheckingList.SignLife = SIGNLIFE;
	}

	if (CheckingList.Attacking == 0 &&
		CheckingList.Move1 == 0 &&
		CheckingList.Move2 == 0)
	{
		if ((!WEInput::IsRepeating(WE_GAMEPAD_A) && WEInput::IsPressed(WE_GAMEPAD_A)) || 
			(!WEInput::IsRepeating(WE_SPACE) && WEInput::IsPressed(WE_SPACE)))
		{
			WEStateManager::SetNextState(STATE_UPGRADE_LEVEL);
		}
	}
	
	//Process Player
	Player->PlayerProcess(dt, &TutorialTileMap, *ObjectManager, *TimeManager, Camera, GameData->SFXVolume);

	point Offset = { ((TutorialTileMap.CountX) * 0.5f * TutorialTileMap.TileWidth - (Player->Pos.X - TutorialTileMap.DownLeftPos.X)) * VISUAL_DOWN_MOVE_SPEED,
		((TutorialTileMap.CountY) * 0.5f * TutorialTileMap.TileHeight - (Player->Pos.Y - TutorialTileMap.DownLeftPos.Y)) * VISUAL_DOWN_MOVE_SPEED };

	//Process Chaser
	for (int Index = 0; Index < ObjectManager->NumChaser; Index++)
		ObjectManager->EnemyChasers[Index].EnemyChaserProcess(Player, dt, &TutorialTileMap, GameData->SFXVolume);

	//Process Turret
	for (int Index = 0; Index < ObjectManager->NumTurret; Index++)
		ObjectManager->EnemyTurrets[Index].EnemyTurretProcess(Player, dt);

	//Process DeathTile
	for (int Index = 0; Index < TileManager->NumDeath; Index++)
		TileManager->DeathTiles[Index].DeathTileProcess(Player);
	//Process MoveTile
	for (int Index = 0; Index < TileManager->NumMove; Index++)
		TileManager->MoveTiles[Index].MoveForPlayer(Player, dt);

	//Ceiling
	CeilingTxt.Pos = { Offset.X * 1.4f,
		Offset.Y * 1.4f };

	//Light_yellow
	YellowLight.Pos = { 5.5f + Offset.X * 1.4f,
		30.5f + Offset.Y * 1.4f };

	//Update Particles
	Emitters->UpdateParticles(dt);

	//Update UI
	UI->UpdateUI(Player);

	Camera.Update({ -Offset.X, -Offset.Y }, dt);
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

	/*Draw Player!!*/

	//Player Shadow
	WEGraphics::SetTextureColor(0xffffffff);
	WEGraphics::SetTexture(PlayerShadowTxt);
	WEMtx44::MakeTransform(transform, Player->DirX * -10.0f, 10.0f,
		0.0f,
		Player->Pos.X - Player->DirX * 0.5f, Player->Pos.Y - 1.4f,
		0.0f);

	WEGraphics::Draw(transform);

	if (Player->AttackAnimationOn && Player->AttackingTime >= 0.0f)
	{

		WEGraphics::SetTextureColor((unsigned char)(253),
			(unsigned char)(32),
			(unsigned char)(154),
			(unsigned char)(150));
		if (Player->AttackDirY == 0)
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

	if (Player->DashType == 1)
	{
		WEGraphics::SetTextureCoords(1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		for (int i = 0;
			i < Player->PlayerIllusionCount;
			++i)
		{
			WEGraphics::SetTextureColor((unsigned char)(60 + i * 5),
				(unsigned char)(150 + i * 5),
				(unsigned char)(110 + i * 5),
				(unsigned char)(180 + i * 5));
			WEGraphics::SetTexture(PlayerDashIllusionTxt[i]);
			WEMtx44::MakeTransform(transform, Player->DashDirX * 30.0f, 15.0f,
				0.0f,
				Player->PlayerIllusion[i].X - Player->DashDirY * Player->DashDirX * 3.5f,
				Player->PlayerIllusion[i].Y + PLAYER_OFFSET_Y,
				0.0f);

			WEGraphics::Draw(transform);
		}
	}

	//player animation
	switch (Player->State)
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
		if (Player->PlayerIllusionCount > 0)
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

	if (Player->Invincible)
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

		if (Particle)
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

	//Draw map_ceiling
	WEGraphics::SetTextureColor(0xffffffff);
	WEGraphics::SetTexture(CeilingTxt.TextureID);
	WEMtx44::MakeTransform(transform, 110.0f, 110.0f,
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

	//Draw UI
	UI->DrawUI(BlankTxt.TextureID);

	//For Tutorial
	char Text[50];
	WEVec2 TopLeft;
	WEVec2 BottomRight;
	WEVec2 TopRight;
	WEGraphics::GetWorldTopLeft(TopLeft);
	TopLeft.x += 3.0f;
	WEGraphics::GetWorldBotRight(BottomRight);
	WEGraphics::GetWorldTopRight(TopRight);
	float YOffSet = 4.0f;
	#define CompleteColor 0xff54fe04
	#define BasicColor 0xff6654ff
	#define KeyColor 0xff92ffd9
	#define WhiteColor 0xffffffff

	switch (CheckingList.Move1)
	{
	case 0:
		sprintf_s(Text, "1. Move 1 Tile : Complete");
		WEGraphics::SetTextureColor(CompleteColor);
		break;
	case 1:
		sprintf_s(Text, "1. Move 1 Tile : 1 / 3");
		WEGraphics::SetTextureColor(BasicColor);
		break;
	case 2:
		sprintf_s(Text, "1. Move 1 Tile : 2 / 3");
		WEGraphics::SetTextureColor(BasicColor);
		break;
	case 3:
		sprintf_s(Text, "1. Move 1 Tile : 3 / 3");
		WEGraphics::SetTextureColor(BasicColor);
		break;
	}

	TopLeft.y -= YOffSet;
	WEGraphics::WriteText(Text, TopLeft.x, TopLeft.y);

	sprintf_s(Text, "( Left Trigger(SHIFT) + Left Stick(WASD) )");
	if(CheckingList.Move1 > 0)
		WEGraphics::SetTextureColor(KeyColor);
	else
		WEGraphics::SetTextureColor(CompleteColor);
	TopLeft.y -= YOffSet;
	WEGraphics::WriteText(Text, TopLeft.x, TopLeft.y);

	switch (CheckingList.Move2)
	{
	case 0:
		sprintf_s(Text, "2. Move 2 Tile : Complete");
		WEGraphics::SetTextureColor(CompleteColor);
		break;
	case 1:
		sprintf_s(Text, "2. Move 2 Tile : 1 / 3");
		WEGraphics::SetTextureColor(BasicColor);
		break;
	case 2:
		sprintf_s(Text, "2. Move 2 Tile : 2 / 3");
		WEGraphics::SetTextureColor(BasicColor);
		break;
	case 3:
		sprintf_s(Text, "2. Move 2 Tile : 3 / 3");
		WEGraphics::SetTextureColor(BasicColor);
		break;
	}
	TopLeft.y -= YOffSet;
	TopLeft.y -= YOffSet;
	WEGraphics::WriteText(Text, TopLeft.x, TopLeft.y);

	sprintf_s(Text, "( Left Stick(WASD) )");
	if (CheckingList.Move2 > 0)
		WEGraphics::SetTextureColor(KeyColor);
	else
		WEGraphics::SetTextureColor(CompleteColor);
	TopLeft.y -= YOffSet;
	WEGraphics::WriteText(Text, TopLeft.x, TopLeft.y);

	switch (CheckingList.Attacking)
	{
	case 0:
		sprintf_s(Text, "3. Attack : Complete");
		WEGraphics::SetTextureColor(CompleteColor);
		break;
	case 1:
		sprintf_s(Text, "3. Attack : 1 / 3");
		WEGraphics::SetTextureColor(BasicColor);
		break;
	case 2:
		sprintf_s(Text, "3. Attack : 2 / 3");
		WEGraphics::SetTextureColor(BasicColor);
		break;
	case 3:
		sprintf_s(Text, "3. Attack : 3 / 3");
		WEGraphics::SetTextureColor(BasicColor);
		break;
	}
	TopLeft.y -= YOffSet;
	TopLeft.y -= YOffSet;
	WEGraphics::WriteText(Text, TopLeft.x, TopLeft.y);

	sprintf_s(Text, "( Right Stick(ARROW KEYS) )");
	if (CheckingList.Attacking > 0)
		WEGraphics::SetTextureColor(KeyColor);
	else
		WEGraphics::SetTextureColor(CompleteColor);
	TopLeft.y -= YOffSet;
	WEGraphics::WriteText(Text, TopLeft.x, TopLeft.y);

	if (CheckingList.SignLife > 0.0f)
	{
		sprintf_s(Text, "Red Box is your Health");
		WEGraphics::SetTextureColor(WhiteColor);
		TopLeft.y -= YOffSet;
		TopLeft.y -= YOffSet;
		WEGraphics::WriteText(Text, TopLeft.x, TopLeft.y);

		CheckingList.SignLife -= dt;

		if (CheckingList.SignLife < 0.0f)
		{
			CheckingList.SignLife = SIGNLIFE;
		}
	}

//	if (CheckingList.IsEnergyExhausted && CheckingList.SignLife > 0.0f)
//	{
		sprintf_s(Text, "Green Box is Your Energy");
		WEGraphics::SetTextureColor(WhiteColor);
		TopLeft.y -= YOffSet;
		WEGraphics::WriteText(Text, TopLeft.x, TopLeft.y);

		sprintf_s(Text, "If you run out of your energy,");
		WEGraphics::SetTextureColor(WhiteColor);
		TopLeft.y -= YOffSet;
		WEGraphics::WriteText(Text, TopLeft.x, TopLeft.y);

		sprintf_s(Text, "you cannot move anymore.");
		WEGraphics::SetTextureColor(WhiteColor);
		TopLeft.y -= YOffSet;
		WEGraphics::WriteText(Text, TopLeft.x, TopLeft.y);

		CheckingList.SignLife -= dt;

		if (CheckingList.SignLife < 0.0f)
		{
			CheckingList.SignLife = SIGNLIFE;
			CheckingList.IsEnergyExhausted = false;
		}
//	}

	if (CheckingList.Attacking == 0 &&
		CheckingList.Move1 == 0 &&
		CheckingList.Move2 == 0)
	{
		sprintf_s(Text, "Press A(SPACE)");
		WEGraphics::SetTextureColor(CompleteColor);
		TopLeft.y -= YOffSet;
		TopLeft.y -= YOffSet;
		WEGraphics::WriteText(Text, TopLeft.x, TopLeft.y);

		sprintf_s(Text, "to play main game");
		WEGraphics::SetTextureColor(CompleteColor);
		TopLeft.y -= YOffSet;
		WEGraphics::WriteText(Text, TopLeft.x, TopLeft.y);
	}


	WEGraphics::EndDraw();

	PlayBGM.channel->setVolume(0.005f * GameData->BGMVolume);
	PlaySFX.channel->setVolume(0.005f * GameData->SFXVolume);
	PlayBGM.system->update();
	PlaySFX.system->update();
}

void tutorialShutdown()
{
	Player->PlayerSFX.system->close();
	Player->PlayerSFX.system->release();
	delete Player;
	ObjectManager->DestroyObjects();
	delete ObjectManager;
	TileManager->DeathTiles;
	delete TileManager;
	Emitters->DeleteParticles();
	delete Emitters;
	delete TimeManager;
	delete UI;

	PlayBGM.system->close();
	PlayBGM.system->release();

	PlaySFX.system->close();
	PlaySFX.system->release();
}

void tutorialUnload()
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
}