/******************************************************************************
FileName: owner.h
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef OWNER_H

#include "WEStateManager.h"
#include "WEInput.h"
#include "WEApplication.h"

#include "hreaper_general.h"
#include "states_enum.h"
#include "player.h"

#include <cstdio>

class item
{
public:
	int Price;
	bool IsPossibleToBuy;
};

class owner
{
public:
	//Content
	point Pos;
	item Shield;
	item Bullet;
	char Text[20];
	int MaxDrawableBoxes;

	//Association
	WEGameData *GameData;
	player* Player;

	//Selector
	int SelectedNum;
	int CurrentScroll; //For Changing Level
	int SelectorState;
	bool IsSelectorOn;
	bool InputLock;


	void Initialize(WEGameData * InputGameData, player* InputPlayer);
	void Process();
	void BuyShield();
	void BuyBullet();
	void SetText(int Index);
	void Draw(WEMtx44& transform);
	void ChangeToPlayLevel();
};




#define OWNER_H
#endif