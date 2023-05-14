/******************************************************************************
FileName: shop.cpp
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "shop.h"

#include "WEInput.h"
#include "WEStateManager.h"
#include "WEApplication.h"
#include "WEGameData.h"
#include "WEMtx44.h"
#include "WEGraphics.h"
#include "WEVec2.h"

#include "player.h"
#include "level_info.h"
#include "object_manager.h"
#include "states_enum.h"
#include "draw_texture.h"
#include "object_manager.h"
#include "time_manager.h"
#include "camera.h"
#include "owner.h"
#include "fmod_general.h"
#include <cstdio>

#include <windows.h>
#include <stdio.h>
namespace
{
	shop Shop;

	object_manager ObjectManager;
	time_manager *TimeManager;
	player* Player;
	WEGameData *GameData;
	owner Owner;

	camera Camera;
	/*Textures*/
	int BlankTxt;
	texture_info Keys;
	//Player Texture
	int PlayerIdleTxt;
	int PlayerTxt;
	int PlayerDashIllusionTxt[7];
	int PlayerPistolTxt;
	int PlayerDashArrowTxt;
	int PlayerShadowTxt;
	fmod_sound_system PlayBGM;
	//Map Texture
	texture_info Ceiling;
	texture_info Floor;

	#define CurrentTileMap Shop.ShopLevel.TileMap
	#define VISUAL_DOWN_MOVE_SPEED 0.04f
	#define PLAYER_OFFSET_Y 4.0f

	uint ShopTileMap[7][7]
	{
		{ 1, 1, 1, 1, 1, 1, 1},
		{ 1, 7, 0, 0, 0, 0, 1},
		{ 1, 0, 0, 0, 0, 0, 1},
		{ 1, 0, 0, 1, 0, 0, 1},
		{ 1, 1, 1, 1, 1, 1, 1},
		{ 1, 1, 1, 1, 1, 1, 1},
		{ 1, 1, 1, 1, 1, 1, 1}
	};
}

void ShopLoad()
{
	PlayerIdleTxt = WEGraphics::LoadTexture("Textures\\Player\\player_idle.tga");
	PlayerTxt = WEGraphics::LoadTexture("Textures\\Player\\player_dash_right2.tga");
	PlayerDashIllusionTxt[0] = WEGraphics::LoadTexture("Textures\\Player\\player_dash_illusion_0.tga");
	PlayerDashIllusionTxt[1] = WEGraphics::LoadTexture("Textures\\Player\\player_dash_illusion_3.tga");
	PlayerDashIllusionTxt[2] = WEGraphics::LoadTexture("Textures\\Player\\player_dash_illusion_4.tga");
	PlayerDashIllusionTxt[3] = WEGraphics::LoadTexture("Textures\\Player\\player_dash_illusion_5.tga");
	PlayerDashIllusionTxt[4] = PlayerDashIllusionTxt[3];
	PlayerDashIllusionTxt[5] = PlayerDashIllusionTxt[3];
	PlayerDashIllusionTxt[6] = PlayerDashIllusionTxt[2];
	PlayerPistolTxt = WEGraphics::LoadTexture("Textures\\Player\\Guns\\player_pistol.tga");
	PlayerShadowTxt = WEGraphics::LoadTexture("Textures\\Player\\player_shadow.tga");
	BlankTxt = WEGraphics::LoadTexture("Textures\\Blank.tga");

	Floor.TextureID = WEGraphics::LoadTexture("Textures\\Maps\\Shop\\shop_floor.tga");
	Ceiling.TextureID = WEGraphics::LoadTexture("Textures\\Maps\\Shop\\shop_ceiling.tga");	
	Keys.TextureID = WEGraphics::LoadTexture("Textures\\keys.tga");
	
}

void ShopInit()
{
	GameData = WEStateManager::GetGameData();

	//Because we don't want to change the gamedata itself
	//But we do want to have infinite dash in the shop.
	WEGameData GameDataRef = *GameData;
	GameDataRef.PlayerEnergy = 10000;
	
	int TileCountX = 7;
	int TileCountY = 7;
	float TileWidth = 15.8f;
	float TileHeight = TileWidth * 0.8f;
	int *TileMapPointer = (int *)ShopTileMap;

	Shop.ShopLevel.Initialize(TileCountX, TileCountY, //TileMapSize
						    {-1 * (TileCountX) * 0.5f * TileWidth, 
							-1 * (TileCountY) * 0.5f * TileHeight}, //DownLeftPos
						    TileWidth, TileHeight, //TileSize
							TileMapPointer, &ObjectManager);
	
	Player = new player(Shop.ShopLevel.InitPlayerPos, 
						Shop.ShopLevel.TileMap.TileWidth, Shop.ShopLevel.TileMap.TileHeight,
						Shop.ShopLevel.TileMap.CountX, Shop.ShopLevel.TileMap.CountY, 
						Shop.ShopLevel.TileMap.DownLeftPos,
						&GameDataRef);
	WEGraphics::SetBackgroundColor(0.098039f, 0.094117f, 0.113725f);

	Owner.Initialize(GameData, Player);

	Floor.Pos = {0.0f, 0.0f};
	Ceiling.Pos = {};

	PlayBGM.MakeSound("Sounds\\BGM\\peace.mp3", true);
	PlayBGM.PlaySound();
	TimeManager = new time_manager(0.01f, 0.0f);
	Keys.Pos = {13.0f, -50.0f};
}

void ShopUpdate(float dt)
{
	WEMtx44 transform;

	//Set Top-Left Position
	WEVec2 topLeft;
	float YOffSet = 4.0f;

	WEGraphics::GetWorldTopLeft(topLeft);

	//Owner Process
	Owner.Process();

	//Player Process(Movement)
	if (!Owner.IsSelectorOn)
	{
		Player->PlayerProcess(dt, &Shop.ShopLevel, ObjectManager, *TimeManager, Camera);
	}

	//Cieling Position
	point Offset = { ((CurrentTileMap.CountX) * 0.5f * CurrentTileMap.TileWidth - (Player->Pos.X - CurrentTileMap.DownLeftPos.X)) * VISUAL_DOWN_MOVE_SPEED,
					 ((CurrentTileMap.CountY) * 0.5f * CurrentTileMap.TileHeight - (Player->Pos.Y - CurrentTileMap.DownLeftPos.Y)) * VISUAL_DOWN_MOVE_SPEED };

	Ceiling.Pos = {Offset.X * 1.4f, 
				   Offset.Y * 1.4f};

	//Set Camera
	WEGraphics::SetCamera(-Offset.X, -Offset.Y, 70.0f, 0.0f);

	WEGraphics::SetToPerspective();

	//Draw Tiles
	WEGraphics::StartDraw();

	//Draw Floor
	WEGraphics::SetTextureColor(0xffffffff);
    WEGraphics::SetTexture(Floor.TextureID);
    WEMtx44::MakeTransform(transform, 111.0f, 111.0f,
                            0.0f,
                            Floor.Pos.X, Floor.Pos.Y,
                            0.0f);

	WEGraphics::Draw(transform);

	//Draw Player Dash Illusion, if there's any.
	WEGraphics::SetTextureCoords(1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0;
		i < Player->PlayerIllusionCount;
		++i)
	{
		WEGraphics::SetTextureColor((unsigned char)(140 + i * 5),
			(unsigned char)(44 + i * 5),
			(unsigned char)(150 + i * 5),
			(unsigned char)(180 + i * 5));
		WEGraphics::SetTexture(PlayerDashIllusionTxt[i]);
		WEMtx44::MakeTransform(transform, Player->DirX * 30.0f, 15.0f,
			0.0f,
			Player->PlayerIllusion[i].X - Player->DirY * Player->DirX * 3.5f,
			Player->PlayerIllusion[i].Y + 3.0f,
			0.0f);

		WEGraphics::Draw(transform);
	}

	//Draw Player Shadow
	WEGraphics::SetTextureColor(0xffffffff);
    WEGraphics::SetTexture(PlayerShadowTxt);
    WEMtx44::MakeTransform(transform, Player->DirX * -10.0f, 10.0f,
                            0.0f,
							Player->Pos.X - Player->DirX * 0.5f, Player->Pos.Y - 1.0f,
                            0.0f);

	WEGraphics::Draw(transform);

	switch(Player->State)
	{
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

	//Draw Player
	WEGraphics::SetTextureColor(0xffffffff);
	
	WEMtx44::MakeTransform(transform, Player->DirX * 30.0f, 15.0f,
							0.0f,
							Player->Pos.X,
							Player->Pos.Y + PLAYER_OFFSET_Y,
							0.0f);
	
	WEGraphics::Draw(transform);



	WEGraphics::SetTexture(BlankTxt);
	WEGraphics::SetTextureCoords(1.0f, 1.0f, 0.0f, 0.0f, 0.0f);

	//Draw Owner
	Owner.Draw(transform);

	//Draw Ceiling
	if (!Owner.IsSelectorOn)
	{
		WEGraphics::SetTextureColor(0xffffffff);
		WEGraphics::SetTexture(Ceiling.TextureID);
		WEMtx44::MakeTransform(transform, 110.0f, 110.0f,
			0.0f,
			Ceiling.Pos.X, Ceiling.Pos.Y,
			0.0f);

		WEGraphics::Draw(transform);


	}
	WEGraphics::SetTexture(Keys.TextureID);
	WEMtx44::MakeTransform(transform, 60.0f, 30.0f,
		0.0f,
		Keys.Pos.X, Keys.Pos.Y,
		0.0f);

	WEGraphics::Draw(transform);
	//Draw Money Text
	char Text[20];
	sprintf_s(Text, "Gold: %.1d", GameData->Money);
	WEGraphics::SetTextureColor(0xFF55ccff);
	topLeft.y -= YOffSet;
	WEGraphics::WriteText(Text, topLeft.x, topLeft.y);

	//Draw Item1 Text
	sprintf_s(Text, "Shield: %.1d", GameData->PlayerShield);
	WEGraphics::SetTextureColor(0xFFfaF28d);
	topLeft.y -= YOffSet;
	WEGraphics::WriteText(Text, topLeft.x, topLeft.y);

	WEGraphics::EndDraw();
}

void ShopShutdown()
{
	delete[] Player;
	delete TimeManager;
	PlayBGM.system->close();
	PlayBGM.system->release();
}
void ShopUnload()
{
	WEGraphics::UnloadTexture(BlankTxt);

	WEGraphics::UnloadTexture(PlayerIdleTxt);
	WEGraphics::UnloadTexture(PlayerTxt);
	WEGraphics::UnloadTexture(PlayerDashArrowTxt);
	WEGraphics::UnloadTexture(PlayerPistolTxt);

	WEGraphics::UnloadTexture(PlayerDashIllusionTxt[0]);
	WEGraphics::UnloadTexture(PlayerDashIllusionTxt[1]);
	WEGraphics::UnloadTexture(PlayerDashIllusionTxt[2]);
	WEGraphics::UnloadTexture(PlayerDashIllusionTxt[3]);
	WEGraphics::UnloadTexture(PlayerDashIllusionTxt[4]);
	WEGraphics::UnloadTexture(PlayerShadowTxt);
	WEGraphics::UnloadTexture(Floor.TextureID);
	WEGraphics::UnloadTexture(Ceiling.TextureID);
	WEGraphics::UnloadTexture(Keys.TextureID);	
}