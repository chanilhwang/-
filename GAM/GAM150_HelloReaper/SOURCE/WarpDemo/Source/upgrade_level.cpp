/******************************************************************************
FileName: upgrade_level.cpp
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

//For seed(time)
#include <ctime>
#include <cstdio>

#include "upgrade_level.h"

#include "player.h"
#include "draw_texture.h"
#include "states_enum.h"

void CheckSelectorBoundary(int &);
void CheckUpgrade(int &);

namespace
{
   WEGameData *GameData;
   
   int SelectedNum;

   point UpgradesPos[3];
   int Upgrades[3];

   bool StickLock;

   //Textures
   int BlankTxt;
   int UpgradeTxt[7];
   #define MAX_UPGRADE_NUM 3
   #define MAX_UPGRADE_VARIETY 6
   #define EACH_UPGRADE_DISTANCE 30.0f
}
void UpgradeLevelLoad()
{
   BlankTxt = WEGraphics::LoadTexture("Textures\\Blank.tga");
   UpgradeTxt[0] = WEGraphics::LoadTexture("Textures\\Upgrades\\HealthUpgrade.tga");
   UpgradeTxt[1] = WEGraphics::LoadTexture("Textures\\Upgrades\\EnergyUpgrade.tga");
   UpgradeTxt[2] = WEGraphics::LoadTexture("Textures\\Upgrades\\SacrificeUpgrade.tga");
   UpgradeTxt[3] = WEGraphics::LoadTexture("Textures\\Upgrades\\LineDashUpgrade.tga");
   UpgradeTxt[4] = WEGraphics::LoadTexture("Textures\\Upgrades\\InvincibleUpgrade.tga");
   UpgradeTxt[5] = WEGraphics::LoadTexture("Textures\\Upgrades\\AwesomeUpgrade.tga");
   UpgradeTxt[6] = WEGraphics::LoadTexture("Textures\\Upgrades\\DashDistanceUpgrade.tga");
}

void UpgradeLevelInit()
{
   WERandom::Seed((int)time(0));

   UpgradesPos[0] = {-EACH_UPGRADE_DISTANCE, 0.0f};
   UpgradesPos[1] = {0.0f, 0.0f};
   UpgradesPos[2] = {EACH_UPGRADE_DISTANCE, 0.0f};

   SelectedNum = 0;

   StickLock = false;

   GameData = WEStateManager::GetGameData();

   Upgrades[0] = WERandom::GetInt(-1, MAX_UPGRADE_VARIETY);
   Upgrades[1] = WERandom::GetInt(-1, MAX_UPGRADE_VARIETY);
   Upgrades[2] = WERandom::GetInt(-1, MAX_UPGRADE_VARIETY);

   while(GameData->PlayerUpgrades[Upgrades[0]] == 1)
   {
      Upgrades[0] = WERandom::GetInt(-1, MAX_UPGRADE_VARIETY);
   }

   while(GameData->PlayerUpgrades[Upgrades[1]] == 1 || Upgrades[1] == Upgrades[0])
   {
      Upgrades[1] = WERandom::GetInt(-1, MAX_UPGRADE_VARIETY);
   }
   
   while(GameData->PlayerUpgrades[Upgrades[2]] == 1 || Upgrades[2] == Upgrades[1] || Upgrades[2] == Upgrades[0])
   {
      Upgrades[2] = WERandom::GetInt(-1, MAX_UPGRADE_VARIETY);
   }
}

void UpgradeLevelUpdate(float dt)
{
   //Process Player Input
   WEVec2 LeftThumb;
   WEInput::GetLeftThumb(LeftThumb);

   WEVec2 AbsLeftThumb = LeftThumb;
   if(LeftThumb.x < 0.0f)
      AbsLeftThumb.x *= -1.0f;

   if(LeftThumb.y < 0.0f)
      AbsLeftThumb.y *= -1.0f;

   if (AbsLeftThumb.x < 0.35f && AbsLeftThumb.y < 0.35f)
		StickLock = false;

   if((!WEInput::IsRepeating(WE_A) && WEInput::IsTriggered(WE_A)) || 
			(!StickLock && LeftThumb.x <= -0.35f && AbsLeftThumb.y < AbsLeftThumb.x))
	{
      --SelectedNum;
		StickLock = true;

      CheckSelectorBoundary(SelectedNum);      
	}
   else if((!WEInput::IsRepeating(WE_D) && WEInput::IsTriggered(WE_D)) || 
			(!StickLock && LeftThumb.x >= 0.35f && AbsLeftThumb.y < AbsLeftThumb.x))
	{
      ++SelectedNum;      
		StickLock = true;

      CheckSelectorBoundary(SelectedNum);      
	}

   if((!WEInput::IsRepeating(WE_SPACE) && WEInput::IsTriggered(WE_SPACE)) || 
		(!WEInput::IsRepeating(WE_GAMEPAD_A) && WEInput::IsTriggered(WE_GAMEPAD_A)))
	{

      switch(Upgrades[SelectedNum])
      {
         default:
         case 0:
         GameData->PlayerShield++;
         break;
         case 1:
         GameData->PlayerEnergy++;
         break;
         case 2:
         GameData->PlayerEnergy += 2;
         GameData->PlayerShield--;
         GameData->PlayerUpgrades[Upgrades[SelectedNum]] = 1;
         break;
         case 3:
         GameData->PlayerUpgrades[Upgrades[SelectedNum]] = 1;
         break;
         case 4:
         GameData->PlayerUpgrades[Upgrades[SelectedNum]] = 1;
         break;
         case 5:
         GameData->PlayerUpgrades[Upgrades[SelectedNum]] = 1;
         GameData->PlayerShield++;
         break;
         case 6:
         GameData->PlayerUpgrades[Upgrades[SelectedNum]] = 1;
         break;
      }
      GameData->CurrentLevelID++;

      WEStateManager::SetNextState(STATE_GAME_PLAYING);
	}

   //Start Drawing
	WEGraphics::StartDraw();
	WEMtx44 transform;
      char Text2[20];
      sprintf_s(Text2, "%d", GameData->CurrentLevelID);
      WEGraphics::SetTextureColor(0xFFFFFFFF);
      WEGraphics::WriteText(Text2, -70.0f, - 30.0f);

      //Draw Upgrades
      for(int i = 0;
            i < 3;
            ++i)
      {
            WEGraphics::SetTexture(UpgradeTxt[Upgrades[i]]);	
            if(i == SelectedNum)
            {
            WEMtx44::MakeTransform(transform, 50, 50,
                                    0.0f,
                                    UpgradesPos[i].X, UpgradesPos[i].Y,
                                    0.0f);
            }
            else
            {
            WEMtx44::MakeTransform(transform, 20, 20,
                                    0.0f,
                                    UpgradesPos[i].X, UpgradesPos[i].Y,
                                    0.0f);
            }
            WEGraphics::Draw(transform);
      }

      //Write Upgrade Information
      char Text[100] = {0};
      switch(Upgrades[SelectedNum])
      {
      case 0:
      {
            sprintf_s(Text, "Increse Health by 1");
      }break;
      case 1:
      {
            sprintf_s(Text, "Increse Energy by 1");
      }break;
      case 2:
            sprintf_s(Text, "Increse Energy by 2 and lose 1 Health");
            break;
      case 3:
      {
            sprintf_s(Text, "Add new dash : Dash until the player meets the wall \\ LB(SPACE) + MOVE");
      }break;
      case 4:
      {
            sprintf_s(Text, "Become invincible for a short period of time while dashing");
      }break;
      case 5:
      {
            sprintf_s(Text, "Add more awesomeness!( + 1 Health)");
      }break;
      case 6:
      {
            sprintf_s(Text, "Increase basic player dash distance");
      }break;
      }
      WEGraphics::SetTextureColor(0xFFFFFFFF);
      WEGraphics::WriteText(Text, -55.0f, - 30.0f);

      sprintf_s(Text, "Press A(SPACE) to select");
      WEGraphics::SetTextureColor(0xFF29ffd9);
      WEGraphics::WriteText(Text, -55.0f, - 40.0f);

      WEGraphics::EndDraw();
}

void UpgradeLevelShutdown()
{
   GameData = 0;
}

void UpgradeLevelUnload()
{
      WEGraphics::UnloadTexture(BlankTxt);
      
      for(int i = 0; i < MAX_UPGRADE_VARIETY + 1; ++i)
      {
            WEGraphics::UnloadTexture(UpgradeTxt[i]);
      }
}

void CheckSelectorBoundary(int &CurrentSelector)
{
   if(CurrentSelector < 0)
   {
      CurrentSelector = MAX_UPGRADE_NUM - 1;
   }

   if(CurrentSelector > MAX_UPGRADE_NUM - 1)
      CurrentSelector = 0;
}