/******************************************************************************
FileName: player.cpp
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "WEInput.h"
#include "WEVec2.h"
#include "WEMath.h"
#include "player.h"
#include "hreaper_intrinsics.h"
#include "collision.h"

namespace
{
	int PrevHealth;

	//Player Upgrades
	int PlayerLineDash;
	int PlayerInvincibleDash;
	int PlayerAwesome; 
	
	#define DASH_COOLDOWN 0.28f
	#define ATTACK_COOLDOWN 0.4f
	
	#define HIT_STOP_LENGTH 0.115f
	#define ATTACK_LENGTH 2.6f
	#define EACH_FRAME_TIME 0.05f
	#define EACH_ATTACK_FRAME_TIME 0.06f

	#define CAMERA_SHAKE_TIME 0.20f
	#define CAMERA_SHAKE_AMOUNT 0.3f
}

void player::Initialize(point InitTilePos,
			   			float InputTileWidth, float InputTileHeight,
						int _CountX, int _CountY,
			   			point DownLeftPos, WEGameData *GameData)
{
	PlayerSFX.MakeSound("Sounds\\SFX\\gunFire.mp3", false);

	Pos.X = (InitTilePos.X - _CountX / 2)  * InputTileWidth;
	Pos.Y = (InitTilePos.Y - _CountY / 2) * InputTileHeight;

	//Because the player offset is a little bit high, we need this.
	float HeightGrow = 0.2f * InputTileHeight;
	Pos.Y += HeightGrow;

	PlayerTileX = RoundFloatToInt(InitTilePos.X + 0.1f);
	PlayerTileY = RoundFloatToInt(InitTilePos.Y + 0.1f);

	Destination = {0.0f, 0.0f};

	Health = 2 + GameData->PlayerShield;
	PrevHealth = Health;
	MaxEnergy = GameData->PlayerEnergy;
	Energy = MaxEnergy;
	
	//Figure out player upgrades
	PlayerLineDash = GameData->PlayerUpgrades[3];
	PlayerInvincibleDash = GameData->PlayerUpgrades[4];
	PlayerAwesome = GameData->PlayerUpgrades[5];

	if(GameData->PlayerUpgrades[6])
		DashUpgrade = 1.0f;
	else
		DashUpgrade = 0.0f;
	Width = 0.35f * InputTileWidth;
	Height = 0.35f * InputTileHeight + HeightGrow;
	
	MovingTime = -1.0f;
	AttackingTime = -1.0f;

	Visible = true;
	id = 1;
	Invincible =  false;
	IsInfiniteDashOn = false;
	GodMode = false;
	State = 0;
	DirX = 1;
	DirY = 1;
	DashDirX = 1;
	DashDirY = 0;
	TextCoord = 0.0f;
	TotalTime = 0.0f;

	TotalAttackTime = 0.0f;
	AttackTextCoord = 0.0f;
	AttackDirX = 0;
	AttackDirY = 0;
	GunPos = Pos;
	PlayerIllusionCount = 0;
}

float 
AssignDashType(player &Player, int StartTile, int EndTile)
{
	float DashLength = 2.0f + Player.DashUpgrade;
	Player.DashType = 0;
	if (((!WEInput::IsRepeating(WE_SHIFT) && WEInput::IsPressed(WE_SHIFT)) || 
		(WEInput::GetLeftTrigger() >= 0.35f)))
	{
      DashLength = 1.0f;      
	}
   
   if(((!WEInput::IsRepeating(WE_SPACE) && WEInput::IsPressed(WE_SPACE)) || 
		(WEInput::IsPressed(WE_GAMEPAD_LEFT_SHOULDER))) && PlayerLineDash == 1)
   {
      DashLength = (float)(EndTile - StartTile);
		Player.DashType = 1;
   }
	return DashLength;
}

bool 
IsTileMapPointEmpty(int PlayerTileX, int PlayerTileY,
					int NewPlayerTileX, int NewPlayerTileY,
					tile_map *CurrentTileMap)
{
	bool IsValid = false;

	if((NewPlayerTileX >= 1) && (NewPlayerTileY >= 1) &&
		(NewPlayerTileX < CurrentTileMap->CountX-1) && 
		(NewPlayerTileY < CurrentTileMap->CountY-1) &&
		(PlayerTileX >= 1) && (PlayerTileY >= 1) && 
		(PlayerTileX < CurrentTileMap->CountX) && 
		(PlayerTileY < CurrentTileMap->CountY))
	{
		int TileMapValue = CurrentTileMap->Tiles[NewPlayerTileY * CurrentTileMap->CountX + 
													   NewPlayerTileX];

		IsValid = (TileMapValue != 1);
	}

	return IsValid;
}

player::player(point InitTilePos,
			   float InputTileWidth, float InputTileHeight,
			   int _CountX, int _CountY,
			   point DownLeftPos, WEGameData *GameData)
{
	Initialize(InitTilePos, InputTileWidth, InputTileHeight,
				_CountX, _CountY,
				DownLeftPos, GameData);
}

void 
PrepareMove(int PlayerTileX,
			int PlayerTileY,
			float DashLength,
			player *Player,
			tile_map *CurrentTileMap)
{
	int NewPlayerTileX = PlayerTileX + TruncateFloatToInt((Player->dX) * DashLength);
	int NewPlayerTileY = PlayerTileY + TruncateFloatToInt((Player->dY) * DashLength);

	if (IsTileMapPointEmpty(PlayerTileX, PlayerTileY,
							NewPlayerTileX, NewPlayerTileY,
							CurrentTileMap))
	{
		Player->dX *= (DashLength)* CurrentTileMap->TileWidth;
		Player->dY *= (DashLength)* CurrentTileMap->TileHeight;
		Player->MovingTime = DASH_COOLDOWN;
		Player->Energy -= 1;

		Player->Destination.X = Player->Pos.X + Player->dX;
		Player->Destination.Y = Player->Pos.Y + Player->dY;	
		Player->Start = Player->Pos;
		Player->State = 1;
		Player->PlayerIllusionCount = 0;
		Player->TextCoord = 0.0f;
	}
	else
	{
		Player->dX = 0.0f;
		Player->dY = 0.0f;
		Player->MovingTime = 0.0f;
		Player->State = 0;
	}
}

void player::PlayerMovingInputProcess(float dt, tile_map *CurrentTileMap)
{
	if (MovingTime <= 0.0f)
	{
		State = 0;
		dX = 0.0f;
		dY = 0.0f;

		float DashLength = 2.0f;
		Destination = Pos;		

		WEVec2 LeftThumb = {0};
		if(WEInput::GamePadIsConnected())
			WEInput::GetLeftThumb(LeftThumb);

		WEVec2 AbsLeftThumb = LeftThumb;
		if(LeftThumb.x < 0.0f)
			AbsLeftThumb.x *= -1.0f;

		if(LeftThumb.y < 0.0f)
			AbsLeftThumb.y *= -1.0f;
		
		if(AttackingTime <= 0.0f)
		{
			DashDirX = DirX;
			DashDirY = DirY;
		}
		if (!WEInput::IsRepeating(WE_W) &&
			!WEInput::IsRepeating(WE_S) &&
			!WEInput::IsRepeating(WE_A) &&
			!WEInput::IsRepeating(WE_D))
		{
			if (WEInput::IsTriggered(WE_W) || 
				(LeftThumb.y >= 0.35f && AbsLeftThumb.x < AbsLeftThumb.y))
			{
				if(Energy >= 1)
				{
					dY = 1.0f;
					DashLength = AssignDashType(*this, PlayerTileY, CurrentTileMap->CountY - 2);

					PrepareMove(PlayerTileX, PlayerTileY, DashLength, this, CurrentTileMap);

					DirY = 0;	
					DashDirY = 0;	
				}
			}

			else if (WEInput::IsTriggered(WE_S) || 
					(LeftThumb.y <= -0.35f && AbsLeftThumb.x < AbsLeftThumb.y))
			{
				if(Energy >= 1)
				{
					dY = -1.0f;
					DashLength = AssignDashType(*this, 1, PlayerTileY);

					PrepareMove(PlayerTileX, PlayerTileY, DashLength, this, CurrentTileMap);

					DirY = 0;
					DashDirY = 0;						
				}
			}

			else if (WEInput::IsTriggered(WE_A) || 
					(LeftThumb.x <= -0.35f && AbsLeftThumb.y < AbsLeftThumb.x))
			{
				if(Energy >= 1)
				{
					dX = -1.0f;
					DashLength = AssignDashType(*this, 1, PlayerTileX);

					PrepareMove(PlayerTileX, PlayerTileY, DashLength, this, CurrentTileMap);

					DirX = -1;
					DashDirX = -1;						
				}
			}

			else if (WEInput::IsTriggered(WE_D) || 
					(LeftThumb.x >= 0.35f && AbsLeftThumb.y < AbsLeftThumb.x))
			{
				if(Energy >= 1)
				{
					dX = 1.0f;
					DashLength = AssignDashType(*this, PlayerTileX, CurrentTileMap->CountX - 2);

					PrepareMove(PlayerTileX, PlayerTileY, DashLength, this, CurrentTileMap);

					DirX = 1;
					DashDirX = 1;											
				}
			}
		}
	}
	else
	{
		Pos.X += (1.0f / DASH_COOLDOWN) * 5.0f * dt * (Destination.X - Pos.X);
		Pos.Y += (1.0f / DASH_COOLDOWN) * 5.0f * dt * (Destination.Y - Pos.Y);
		MovingTime -= dt;


		if(MovingTime <= DASH_COOLDOWN && PlayerIllusionCount == 0)
		{
			PlayerIllusionCount = 1;
			PlayerIllusion[0] = Pos;

		}
		else if(MovingTime <= 0.9* DASH_COOLDOWN  && PlayerIllusionCount == 1)
		{
			PlayerIllusionCount = 2;
			PlayerIllusion[1] = Pos;

		}
		else if(MovingTime <= 0.8 * DASH_COOLDOWN  && PlayerIllusionCount == 2)
		{
			PlayerIllusionCount = 3;
			PlayerIllusion[2] = Pos;

		}
		else if(MovingTime <= 0.7 * DASH_COOLDOWN  && PlayerIllusionCount == 3)
		{
			PlayerIllusionCount = 4;
			PlayerIllusion[3] = Pos;

		}
		else if(MovingTime <= 0.6f * DASH_COOLDOWN  && PlayerIllusionCount == 4)
		{
			PlayerIllusionCount = 5;
			PlayerIllusion[4] = Pos;

		}
		else if(MovingTime <= 0.3f * DASH_COOLDOWN  && PlayerIllusionCount == 5)
		{
			PlayerIllusionCount = 5;			
			PlayerIllusion[5] = Pos;			
		}

		//Check whether player should be invincible or not.
		if(PlayerInvincibleDash == 1 && !GodMode)
		{
			if(MovingTime >= 0.6f * DASH_COOLDOWN)
				Invincible = true;
			else
				Invincible = false;
		}

		if(MovingTime <= 0.0f)
		{
			Pos = Destination;
			State = 0;
			TextCoord = 0.0f;
		}
		
		PlayerTileX = 
			RoundFloatToInt((Pos.X + CurrentTileMap->TileWidth * CurrentTileMap->CountX * 0.4f) / 
							 CurrentTileMap->TileWidth);
		PlayerTileY = 
			RoundFloatToInt((Pos.Y + CurrentTileMap->TileHeight * CurrentTileMap->CountY * 0.4f) / 
							 CurrentTileMap->TileHeight);
	}
}

inline 
float GetDistanceSquare(point Pos1, point Pos2)
{
    return ((Pos1.X - Pos2.X) * (Pos1.X - Pos2.X) + 
            (Pos1.Y - Pos2.Y) * (Pos1.Y - Pos2.Y));  
}

//Start : Weapon Attack Processes
void AttackProcess(player &Player, tile_map *CurrentTileMap, object_manager &ObjectManager,
				   float AttackLength, point AttackVector, time_manager &TimeManager, camera &Camera)
{
	enemy *SelectedEnemy = 0;
	float Distance = AttackLength * AttackLength;

	for(int Index = 0;
		Index < ObjectManager.NumChaser;
		++Index)
	{
		if(GetDistanceSquare(Player.Pos, ObjectManager.EnemyChasers[Index].Pos) < Distance)
		{
			point PlayerAttackPos = Player.Pos;
			// PlayerAttackPos.Y += 1.0f;
			if(RayCastR(PlayerAttackPos, AttackVector, ObjectManager.EnemyChasers[Index]))
			{
				if(ObjectManager.EnemyChasers[Index].Visible)
				{
					SelectedEnemy = &ObjectManager.EnemyChasers[Index];
					Distance = GetDistanceSquare(Player.Pos, ObjectManager.EnemyChasers[Index].Pos);
				}
			}
		}
	}
	
	for(int Index = 0;
		Index < ObjectManager.NumTurret;
		++Index)
	{
		if(GetDistanceSquare(Player.Pos, ObjectManager.EnemyTurrets[Index].Pos) < Distance)
		{			
			point PlayerAttackPos = Player.Pos;
			// PlayerAttackPos.Y += 1.0f;
			if(RayCastR(PlayerAttackPos, AttackVector, ObjectManager.EnemyTurrets[Index]))
			{
				if(ObjectManager.EnemyTurrets[Index].Visible)
				{
					SelectedEnemy = &ObjectManager.EnemyTurrets[Index];
				}
			}
		}
	}

	for(int Index = 0;
		Index < ObjectManager.NumSlow;
		++Index)
	{
		if(GetDistanceSquare(Player.Pos, ObjectManager.EnemySlow[Index].Pos) < Distance)
		{			
			point PlayerAttackPos = Player.Pos;
			// PlayerAttackPos.Y += 1.0f;
			if(RayCastR(PlayerAttackPos, AttackVector, ObjectManager.EnemySlow[Index]))
			{
				if(ObjectManager.EnemySlow[Index].Visible)
				{
					SelectedEnemy = &ObjectManager.EnemySlow[Index];
				}
			}
		}
	}

	if(SelectedEnemy != 0)
	{				
		if(PlayerAwesome == 0)
		{
			TimeManager.SlowDownLength = HIT_STOP_LENGTH;					
			Camera.SetShake(CAMERA_SHAKE_TIME, CAMERA_SHAKE_AMOUNT);
		}
		else
		{
			TimeManager.SlowDownLength = 1.5f * HIT_STOP_LENGTH;			
			Camera.SetShake(4 * CAMERA_SHAKE_TIME, 4 * CAMERA_SHAKE_AMOUNT);
		}
		SelectedEnemy->Health -= 1;	
		if(Player.Energy != 5)
			Player.Energy ++;
	}
}

void DashAttackProcess(player &Player, tile_map *CurrentTileMap, object_manager &ObjectManager,
				   float AttackLength, point AttackVector, time_manager &TimeManager, camera &Camera)
{
	enemy *SelectedEnemy[3] = {0};
	int AttackedEnemyNum = 0;
	float Distance = AttackLength * AttackLength;

	for(int Index = 0;
		Index < ObjectManager.NumChaser;
		++Index)
	{
		if(GetDistanceSquare(Player.Pos, ObjectManager.EnemyChasers[Index].Pos) < Distance && AttackedEnemyNum < 3)
		{
			point PlayerAttackPos = Player.Pos;
			// PlayerAttackPos.Y += 1.0f;
			if(RayCastR(PlayerAttackPos, AttackVector, ObjectManager.EnemyChasers[Index]))
			{
				if(ObjectManager.EnemyChasers[Index].Visible)
				{
					SelectedEnemy[AttackedEnemyNum] = &ObjectManager.EnemyChasers[Index];
					Distance = GetDistanceSquare(Player.Pos, ObjectManager.EnemyChasers[Index].Pos);
					// AttackedEnemyNum++;
				}
			}
		}
	}
	
	for(int Index = 0;
		Index < ObjectManager.NumTurret;
		++Index)
	{
		if(GetDistanceSquare(Player.Pos, ObjectManager.EnemyTurrets[Index].Pos) < Distance && AttackedEnemyNum < 3)
		{			
			point PlayerAttackPos = Player.Pos;
			// PlayerAttackPos.Y += 1.0f;
			if(RayCastR(PlayerAttackPos, AttackVector, ObjectManager.EnemyTurrets[Index]))
			{
				if(ObjectManager.EnemyTurrets[Index].Visible)
				{
					SelectedEnemy[AttackedEnemyNum] = &ObjectManager.EnemyTurrets[Index];
					// AttackedEnemyNum++;					
				}
			}
		}
	}

	for(int Index = 0;
		Index < ObjectManager.NumSlow;
		++Index)
	{
		if(GetDistanceSquare(Player.Pos, ObjectManager.EnemySlow[Index].Pos) < Distance && AttackedEnemyNum < 3)
		{			
			point PlayerAttackPos = Player.Pos;
			// PlayerAttackPos.Y += 1.0f;
			if(RayCastR(PlayerAttackPos, AttackVector, ObjectManager.EnemySlow[Index]))
			{
				if(ObjectManager.EnemySlow[Index].Visible)
				{
					SelectedEnemy[AttackedEnemyNum] = &ObjectManager.EnemySlow[Index];
					// AttackedEnemyNum++;										
				}
			}
		}
	}
	for(int i = 0;
		i < AttackedEnemyNum;
		++i)
	{
		
		if(SelectedEnemy[AttackedEnemyNum] != 0)
		{				
			if(PlayerAwesome == 0)
			{
				TimeManager.SlowDownLength = HIT_STOP_LENGTH;					
				Camera.SetShake(CAMERA_SHAKE_TIME, CAMERA_SHAKE_AMOUNT);
			}
			else
			{
				TimeManager.SlowDownLength = 1.5f * HIT_STOP_LENGTH;			
				Camera.SetShake(4 * CAMERA_SHAKE_TIME, 4 * CAMERA_SHAKE_AMOUNT);
			}
			SelectedEnemy[AttackedEnemyNum]->Health -= 1;	
			if(Player.Energy != 5)
				Player.Energy ++;
		}
	}
}
//End : Weapon Attack Processes

void player::PlayerAttackingInputProcess(float dt, tile_map *CurrentTileMap, object_manager &ObjectManager,
										time_manager &TimeManager, camera &Camera, float SFXVolume)
{
	if(AttackingTime < 0.0f)
	{
		WEVec2 RightThumb;
		WEInput::GetRightThumb(RightThumb);
		
		WEVec2 AbsRightThumb = RightThumb;
		if(RightThumb.x < 0.0f)
			AbsRightThumb.x *= -1.0f;

		if(RightThumb.y < 0.0f)
			AbsRightThumb.y *= -1.0f;

		if (!WEInput::IsRepeating(WE_ARROW_LEFT) &&
			!WEInput::IsRepeating(WE_ARROW_DOWN) &&
			!WEInput::IsRepeating(WE_ARROW_UP) &&
			!WEInput::IsRepeating(WE_ARROW_RIGHT))
		{
			if (WEInput::IsTriggered(WE_ARROW_UP) || 
				(RightThumb.y >= 0.35f && AbsRightThumb.x < AbsRightThumb.y))
			{
				float AttackLength = ATTACK_LENGTH * CurrentTileMap->TileHeight;
				
				AttackProcess(*this, CurrentTileMap, ObjectManager, AttackLength, {0.00001f, 1.0f}, TimeManager, Camera);
				
				AttackingTime = ATTACK_COOLDOWN;			
				AttackPos = Pos;
				AttackTextCoord = 0.0f;
				TotalAttackTime = 0.0f;
				DirY = 1;
				AttackDirY = 1;
				AttackAnimationOn = true;
				PlayerSFX.PlaySound();
			}
			else if (WEInput::IsTriggered(WE_ARROW_DOWN) || 
					(RightThumb.y <= -0.35f && AbsRightThumb.x < AbsRightThumb.y))
			{
				float AttackLength = ATTACK_LENGTH * CurrentTileMap->TileHeight;

				AttackProcess(*this, CurrentTileMap, ObjectManager, AttackLength, {0.00001f, -1.0f}, TimeManager, Camera);
				
				AttackingTime = ATTACK_COOLDOWN;		
				AttackPos = Pos;	
				AttackTextCoord = 0.0f;
				TotalAttackTime = 0.0f;
				DirY = -1;
				AttackDirY = -1;			
				AttackAnimationOn = true;
				PlayerSFX.PlaySound();				
			}
			else if (WEInput::IsTriggered(WE_ARROW_RIGHT) || 
					(RightThumb.x >= 0.35f && AbsRightThumb.y < AbsRightThumb.x))
			{
				float AttackLength = ATTACK_LENGTH * CurrentTileMap->TileWidth;
				
				AttackProcess(*this, CurrentTileMap, ObjectManager, AttackLength, {1.0f, 0.00001f}, TimeManager, Camera);
				
				AttackingTime = ATTACK_COOLDOWN;		
				AttackPos = Pos;	
				AttackTextCoord = 0.0f;
				TotalAttackTime = 0.0f;
				DirX = 1;
				AttackDirX = 1;
				AttackDirY = 0;		
				AttackAnimationOn = true;
				PlayerSFX.PlaySound();							
			}
			else if (WEInput::IsTriggered(WE_ARROW_LEFT) || 
					(RightThumb.x <= -0.35f && AbsRightThumb.y < AbsRightThumb.x))
			{
				float AttackLength = ATTACK_LENGTH * CurrentTileMap->TileWidth;
			
				AttackProcess(*this, CurrentTileMap, ObjectManager, AttackLength, {-1.0f, 0.00001f}, TimeManager, Camera);
				
				AttackingTime = ATTACK_COOLDOWN;		
				AttackPos = Pos;				
				AttackTextCoord = 0.0f;
				TotalAttackTime = 0.0f;
				DirX = -1;
				AttackDirX = -1;
				AttackDirY = 0;						
				AttackAnimationOn = true;
				PlayerSFX.PlaySound();						
			}
		}
		//set player sfx volume
		PlayerSFX.channel->setVolume(0.005f * SFXVolume);
	}
	else
	{
		AttackingTime -= dt;
	}
}

void player::PlayerInputProcess(float dt, tile_map* CurrentTileMap, object_manager &ObjectManager,
								time_manager &TimeManager, camera &Camera, float SFXVolume)
{
	PlayerMovingInputProcess(dt, CurrentTileMap);
	PlayerAttackingInputProcess(dt, CurrentTileMap, ObjectManager, TimeManager, Camera, SFXVolume);
}


#include "windows.h"
#include "stdio.h"
void player::PlayerProcess(float dt, tile_map* CurrentTileMap, object_manager &ObjectManager,
							time_manager &TimeManager, camera &Camera, float SFXVolume)
{
	if(State > -1)
		PlayerInputProcess(dt, CurrentTileMap, ObjectManager, TimeManager, Camera, SFXVolume);

	TotalTime += dt;

	if(TotalTime >= EACH_FRAME_TIME)
	{
		TotalTime = 0.0f;
		TextCoord += .125f;
		WEMath::Wrap(TextCoord, 0.0f, 1.0f);
	}

	TotalAttackTime += dt;
	if(TotalAttackTime >= EACH_ATTACK_FRAME_TIME)
	{
		TotalAttackTime = 0.0f;
		AttackTextCoord += 0.125f;
		if(AttackTextCoord >= 1.0f)
			AttackAnimationOn = false;
		WEMath::Wrap(AttackTextCoord, 0.0f, 1.0f);		
	}

	//If damaged, set the effect.
	if(Health < PrevHealth)
	{
		TimeManager.SlowDownLength = 0.8f * HIT_STOP_LENGTH;		
		Camera.SetShake(1.1f * CAMERA_SHAKE_TIME, 1.0f * CAMERA_SHAKE_AMOUNT);
		PrevHealth = Health;
	}

	if(Health <= 0)
	{

		if(State != -1)
		{
			TextCoord = 0.0f;
			// TimeManager.SlowDownLength = 0.2f;				
			State = -1;			
		}
	}
	
	GunPos.X = Pos.X - DirX * 6.0f; 
	GunPos.Y = Pos.Y + 2.0f;
}