/******************************************************************************
FileName: owner.cpp
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "owner.h"
#include <cstdio>
namespace
{
	#define SHIELD_PRICE_OFFSET 2
	#define BULLET_PRICE_OFFSET 2
	#define MAX_SELECTOR_MAIN_NUM 2
	#define MAX_SELECTOR_LEVELSELECT_NUM 3
	#define MAX_SELECTOR_PURCHASE_NUM 1
	#define MAX_SHIELD_NUM 4

	enum SELECTOR_STATES
	{
		SELECTOR_NULL,
		SELECTOR_MAIN,
		SELECTOR_LEVELSELECT,
		SELECTOR_PURCHASE
	};

	enum SELECTOR_MAIN_STATES
	{
		SELECTOR_MAIN_NULL,
		SELECTOR_MAIN_LEVELSELECT,
		SELECTOR_MAIN_PURCHASE,
		SELECTOR_MAIN_GAMESTART
	};

	enum SELECTOR_PURCHASE_STATES
	{
		SELECTOR_PURCHASE_NULL,
		SELECTOR_PURCHASE_SHIELD,
		SELECTOR_PURCHASE_BULLET
	};
}

float CalculateBoxSize(float BasicSize, int numBox)
{
	return (BasicSize) / (2 * MAX_SELECTOR_MAIN_NUM + 1);
}

float CalculateBoxXPos(float BasicSize, int Index, int numBox)
{
	return -((1.f / 2.f * BasicSize) - (4.f / 3.f * Index + 1.f / 3.f) * (BasicSize) / (2 * numBox + 1));
}

void owner::SetText(int Index)
{
	if (SelectorState == SELECTOR_MAIN)
	{
		switch (Index)
		{
		case 1:
		{
			sprintf_s(Text, "Level Select");
		} break;
		case 2:
		{
			sprintf_s(Text, "Purchase");
		} break;
		case 3:
		{
			sprintf_s(Text, "Game Start");
		} break;
		}
	}

	else if (SelectorState == SELECTOR_PURCHASE)
	{
		switch (Index)
		{
		case 1:
		{
			sprintf_s(Text, "SHIELD");
		} break;
		case 2:
		{
			sprintf_s(Text, "BULLET");
		} break;
		}
	}

	else if (SelectorState == SELECTOR_LEVELSELECT)
	{
		sprintf_s(Text, "%.1d", Index);
	}
}

void owner::Initialize(WEGameData * InputGameData, player* InputPlayer)
{
	//Position
	Pos = { 3,2 };

	//Init Items
	Shield.Price = SHIELD_PRICE_OFFSET;
	Shield.IsPossibleToBuy = true;

	Bullet.Price = BULLET_PRICE_OFFSET;
	Bullet.IsPossibleToBuy = true;

	//GameData
	GameData = InputGameData;

	//Player
	Player = InputPlayer;

	//Selector
	SelectedNum = 1;
	CurrentScroll = 3;
	SelectorState = SELECTOR_MAIN;
	IsSelectorOn = false;
	InputLock = false;
	MaxDrawableBoxes = 3;
}

void owner::Process()
{
	WEVec2 LeftThumb;
	WEInput::GetLeftThumb(LeftThumb);
	WEVec2 AbsLeftThumb = LeftThumb;

	if (LeftThumb.x < 0.0f)
		AbsLeftThumb.x *= -1.0f;

	if (LeftThumb.y < 0.0f)
		AbsLeftThumb.y *= -1.0f;

	if (InputLock)
	{
		if (LeftThumb.x < 0.35f && LeftThumb.x > -0.35f)
			InputLock = false;
	}

	//Check Player Moving And Position
	if (Player->State == 0 && !IsSelectorOn)
	{
		if (!WEInput::IsRepeating(WE_E) && WEInput::IsTriggered(WE_E)					||
			(!WEInput::IsRepeating(WE_GAMEPAD_A) && WEInput::IsTriggered(WE_GAMEPAD_A)))
		{
			if ((Player->PlayerTileX == Pos.X && Player->PlayerTileY == Pos.Y - 1) ||
				(Player->PlayerTileX == Pos.X && Player->PlayerTileY == Pos.Y + 1) ||
				(Player->PlayerTileX == Pos.X + 1 && Player->PlayerTileY == Pos.Y) ||
				(Player->PlayerTileX == Pos.X - 1 && Player->PlayerTileY == Pos.Y))
			{
				IsSelectorOn = true;
			}
		}
	}

	else if (IsSelectorOn)
	{
		if ((!WEInput::IsRepeating(WE_D) && WEInput::IsTriggered(WE_D))									||
			(!WEInput::IsRepeating(WE_GAMEPAD_DPAD_LEFT) && WEInput::IsTriggered(WE_GAMEPAD_DPAD_LEFT)) ||
			(LeftThumb.x >= 0.35f && AbsLeftThumb.y < AbsLeftThumb.x)									&&
			!InputLock)
		{
			InputLock = true;
			if (SelectorState == SELECTOR_MAIN)
			{
				if (SelectedNum >= MAX_SELECTOR_MAIN_NUM)
				{
					SelectedNum = 1;
					CurrentScroll = 3;
				}
				else
				{
					SelectedNum += 1;
					if (SelectedNum > CurrentScroll)
						CurrentScroll += 1;
				}
			}

			else if (SelectorState == SELECTOR_LEVELSELECT)
			{
				if (SelectedNum >= MAX_SELECTOR_LEVELSELECT_NUM)
				{
					SelectedNum = 1;
					CurrentScroll = 3;
				}
				else
				{
					SelectedNum += 1;
					if(SelectedNum > CurrentScroll)
						CurrentScroll += 1;
				}
			}

			else if (SelectorState == SELECTOR_PURCHASE)
			{
				if (SelectedNum >= MAX_SELECTOR_PURCHASE_NUM)
				{
					SelectedNum = 1;
					CurrentScroll = 3;
				}
				else
				{
					SelectedNum += 1;
					if (SelectedNum > CurrentScroll)
						CurrentScroll += 1;
				}
			}
		}
		else if ((!WEInput::IsRepeating(WE_A) && WEInput::IsTriggered(WE_A))							  ||
			(!WEInput::IsRepeating(WE_GAMEPAD_DPAD_RIGHT) && WEInput::IsTriggered(WE_GAMEPAD_DPAD_RIGHT)) ||
			(LeftThumb.x <= -0.35f && AbsLeftThumb.y < AbsLeftThumb.x)									  &&
			!InputLock)
		{
			InputLock = true;
			if (SelectorState == SELECTOR_MAIN)
			{
				if (SelectedNum <= 1)
				{
					SelectedNum = MAX_SELECTOR_MAIN_NUM;
					CurrentScroll = MAX_SELECTOR_MAIN_NUM;
				}
				else
				{
					SelectedNum -= 1;
					if (SelectedNum <= CurrentScroll - MaxDrawableBoxes)
						CurrentScroll -= 1;
				}
			}
			else if (SelectorState == SELECTOR_LEVELSELECT)
			{
				if (SelectedNum <= 1)
				{
					SelectedNum = MAX_SELECTOR_LEVELSELECT_NUM;
					CurrentScroll = MAX_SELECTOR_LEVELSELECT_NUM;
				}
				else
				{
					SelectedNum -= 1;
					if (SelectedNum <= CurrentScroll - MaxDrawableBoxes)
						CurrentScroll -= 1;
				}
			}
			else if (SelectorState == SELECTOR_PURCHASE)
			{
				if (SelectedNum <= 1)
				{
					SelectedNum = MAX_SELECTOR_PURCHASE_NUM;
					CurrentScroll = MAX_SELECTOR_PURCHASE_NUM;
				}
				else
				{
					SelectedNum -= 1;
					if (SelectedNum <= CurrentScroll - MaxDrawableBoxes)
						CurrentScroll -= 1;
				}
			}
		}
		
		if ((!WEInput::IsRepeating(WE_E) && WEInput::IsTriggered(WE_E)) ||
			(!WEInput::IsRepeating(WE_GAMEPAD_A) && WEInput::IsTriggered(WE_GAMEPAD_A)))
		{
			if (SelectorState == SELECTOR_MAIN)
			{
				if (SelectedNum == SELECTOR_MAIN_LEVELSELECT)
				{
					SelectedNum = 1;
					SelectorState = SELECTOR_LEVELSELECT;
				}

				else if (SelectedNum == SELECTOR_MAIN_PURCHASE)
				{
					SelectedNum = 1;
					SelectorState = SELECTOR_PURCHASE;
				}

				else if (SelectedNum == SELECTOR_MAIN_GAMESTART)
				{
					ChangeToPlayLevel();
				}
			}

			else if (SelectorState == SELECTOR_LEVELSELECT)
			{
				GameData->CurrentLevelID = SelectedNum;
				SelectorState = SELECTOR_MAIN;
				ChangeToPlayLevel();
			}

			else if (SelectorState == SELECTOR_PURCHASE)
			{
				if (SelectedNum == SELECTOR_PURCHASE_SHIELD)
				{
					BuyShield();
				}

				else if (SelectedNum == SELECTOR_PURCHASE_BULLET)
				{
					BuyBullet();
				}
			}
		}
		else if ((!WEInput::IsRepeating(WE_ESCAPE) && WEInput::IsTriggered(WE_ESCAPE)) ||
			(!WEInput::IsRepeating(WE_GAMEPAD_B) && WEInput::IsTriggered(WE_GAMEPAD_B)))
		{
			if (SelectorState == SELECTOR_MAIN)
			{
				SelectedNum = 1;
				CurrentScroll = 3;
				IsSelectorOn = false;
			}

			else if (SelectorState == SELECTOR_PURCHASE)
			{
				SelectedNum = 1;
				CurrentScroll = 3;
				SelectorState = SELECTOR_MAIN;
			}

			else if (SelectorState == SELECTOR_LEVELSELECT)
			{
				SelectedNum = 1;
				CurrentScroll = 3;
				SelectorState = SELECTOR_MAIN;
			}
		}
	}
}

void owner::Draw(WEMtx44& transform)
{
	if (IsSelectorOn)
	{
		float BasicSize = WEApplication::GetWidth() * 0.075f;

		//Draw Selector Box
		if (SelectorState == SELECTOR_MAIN)
		{
			if (MAX_SELECTOR_MAIN_NUM < MaxDrawableBoxes)
			{
				for (int Index = 1; Index <= MAX_SELECTOR_MAIN_NUM; Index++)
				{
					WEGraphics::SetTextureColor(0xff555555);
					if (SelectedNum == Index)
						WEGraphics::SetTextureColor(0xff6439ff);
					WEMtx44::MakeTransform(transform, CalculateBoxSize(BasicSize, MAX_SELECTOR_MAIN_NUM), CalculateBoxSize(BasicSize, MAX_SELECTOR_MAIN_NUM),
						0.0f,
						CalculateBoxXPos(BasicSize, Index, MAX_SELECTOR_MAIN_NUM),
						0.0f,
						0.0f);
					WEGraphics::Draw(transform);
				}
			}

			else
			{
				for (int Index = 1; Index <= MaxDrawableBoxes; Index++)
				{
					WEGraphics::SetTextureColor(0xff555555);
					if (SelectedNum == Index)
						WEGraphics::SetTextureColor(0xff6439ff);
					WEMtx44::MakeTransform(transform, CalculateBoxSize(BasicSize, MaxDrawableBoxes), CalculateBoxSize(BasicSize, MaxDrawableBoxes),
						0.0f,
						CalculateBoxXPos(BasicSize, Index, MaxDrawableBoxes),
						0.0f,
						0.0f);
					WEGraphics::Draw(transform);
				}
			}
		}

		else if (SelectorState == SELECTOR_PURCHASE)
		{
			if (MAX_SELECTOR_PURCHASE_NUM < MaxDrawableBoxes)
			{
				for (int Index = 1; Index <= MAX_SELECTOR_PURCHASE_NUM; Index++)
				{
					WEGraphics::SetTextureColor(0xff555555);
					if (SelectedNum == Index)
						WEGraphics::SetTextureColor(0xff6439ff);
					WEMtx44::MakeTransform(transform, CalculateBoxSize(BasicSize, MAX_SELECTOR_PURCHASE_NUM), CalculateBoxSize(BasicSize, MAX_SELECTOR_PURCHASE_NUM),
						0.0f,
						CalculateBoxXPos(BasicSize, Index, MAX_SELECTOR_PURCHASE_NUM),
						0.0f,
						0.0f);
					WEGraphics::Draw(transform);
				}
			}

			else
			{
				for (int Index = 1; Index <= MaxDrawableBoxes; Index++)
				{
					WEGraphics::SetTextureColor(0xff555555);
					if (SelectedNum == Index)
						WEGraphics::SetTextureColor(0xff6439ff);
					WEMtx44::MakeTransform(transform, CalculateBoxSize(BasicSize, MaxDrawableBoxes), CalculateBoxSize(BasicSize, MaxDrawableBoxes),
						0.0f,
						CalculateBoxXPos(BasicSize, Index, MaxDrawableBoxes),
						0.0f,
						0.0f);
					WEGraphics::Draw(transform);
				}
			}
		}

		else if (SelectorState == SELECTOR_LEVELSELECT)
		{
			if (MAX_SELECTOR_LEVELSELECT_NUM < MaxDrawableBoxes)
			{
				for (int Index = 1; Index <= MAX_SELECTOR_LEVELSELECT_NUM; Index++)
				{
					WEGraphics::SetTextureColor(0xff555555);
					
					if (SelectedNum == Index)
						WEGraphics::SetTextureColor(0xff6439ff);
					WEMtx44::MakeTransform(transform, CalculateBoxSize(BasicSize, MAX_SELECTOR_LEVELSELECT_NUM), CalculateBoxSize(BasicSize, MAX_SELECTOR_LEVELSELECT_NUM),
						0.0f,
						CalculateBoxXPos(BasicSize, Index, MAX_SELECTOR_LEVELSELECT_NUM),
						0.0f,
						0.0f);
					WEGraphics::Draw(transform);
				}
			}

			else
			{
				for (int Index = 1; Index <= MaxDrawableBoxes; Index++)
				{
					WEGraphics::SetTextureColor(0xff555555);					
					if (CurrentScroll - SelectedNum + Index == 3)
						WEGraphics::SetTextureColor(0xff6439ff);
					WEMtx44::MakeTransform(transform, CalculateBoxSize(BasicSize, MaxDrawableBoxes), CalculateBoxSize(BasicSize, MaxDrawableBoxes),
						0.0f,
						CalculateBoxXPos(BasicSize, Index, MaxDrawableBoxes),
						0.0f,
						0.0f);
					WEGraphics::Draw(transform);
				}
			}
		}

		//Draw Selector Passage
		if (SelectorState == SELECTOR_MAIN)
		{
			if (MAX_SELECTOR_MAIN_NUM < MaxDrawableBoxes)
			{
				for (int Index = 1; Index <= MAX_SELECTOR_MAIN_NUM; Index++)
				{
					SetText(Index);
					WEGraphics::SetTextureColor(0xffffffff);
					WEGraphics::WriteText(Text, CalculateBoxXPos(BasicSize, Index, MAX_SELECTOR_MAIN_NUM) - 10.0f, 0.0f);
				}
			}

			else
			{
				for (int Index = 1; Index <= MaxDrawableBoxes; Index++)
				{
					SetText(Index);
					WEGraphics::SetTextureColor(0xffffffff);
					WEGraphics::WriteText(Text, CalculateBoxXPos(BasicSize, Index, MaxDrawableBoxes), 0.0f);
				}
			}
		}

		else if (SelectorState == SELECTOR_PURCHASE)
		{
			if (MAX_SELECTOR_PURCHASE_NUM < MaxDrawableBoxes)
			{
				for (int Index = 1; Index <= MAX_SELECTOR_PURCHASE_NUM; Index++)
				{
					SetText(Index);
					WEGraphics::SetTextureColor(0xffffffff);
					WEGraphics::WriteText(Text, CalculateBoxXPos(BasicSize, Index, MAX_SELECTOR_PURCHASE_NUM) - 10.0f, 0.0f);
				}
			}

			else
			{
				for (int Index = 1; Index <= MaxDrawableBoxes; Index++)
				{
					SetText(Index);
					WEGraphics::SetTextureColor(0xffffffff);
					WEGraphics::WriteText(Text, CalculateBoxXPos(BasicSize, Index, MaxDrawableBoxes), 0.0f);
				}
			}
		}

		else if (SelectorState == SELECTOR_LEVELSELECT)
		{
			if (MAX_SELECTOR_LEVELSELECT_NUM < MaxDrawableBoxes)
			{
				for (int Index = 1; Index <= MAX_SELECTOR_LEVELSELECT_NUM; Index++)
				{
					SetText(Index);
					WEGraphics::SetTextureColor(0xffffffff);
					WEGraphics::WriteText(Text, CalculateBoxXPos(BasicSize, Index, MAX_SELECTOR_LEVELSELECT_NUM), 0.0f);
				}
			}

			else
			{
				for (int Index = 1; Index <= MaxDrawableBoxes; Index++)
				{
					SetText(CurrentScroll - (3 - Index));
					WEGraphics::SetTextureColor(0xffffffff);
					WEGraphics::WriteText(Text, CalculateBoxXPos(BasicSize, Index, MaxDrawableBoxes), 0.0f);
				}
			}
		}
	}
}

void owner::BuyShield()
{
	if (GameData->Money >= Shield.Price &&
		GameData->PlayerShield < MAX_SHIELD_NUM)
	{
		GameData->PlayerShield += 1;
		GameData->Money -= Shield.Price;
	}
}

void owner::BuyBullet()
{
	if (GameData->Money >= Bullet.Price)
	{
		GameData->PlayerBullet += 1;
		GameData->Money -= Bullet.Price;
	}
}

void owner::ChangeToPlayLevel()
{
	WEStateManager::SetNextState(STATE_GAME_PLAYING);
}