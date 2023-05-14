/******************************************************************************
FileName: enemy.cpp
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "enemy.h"

#include "vector.h"
#include "hreaper_intrinsics.h"
#include "WERandom.h"

namespace
{
    #define ENEMY_CHASER_HEALTH 1
    #define ENEMY_TURRET_HEALTH 1
    
    #define ENEMY_ATTACK_COOLDOWN 1.3f
    #define ENEMY_CHASER_DASH_SPEED 3.0f
    #define ENEMY_CHASER_DASH_LENGTH 35.0f    
    #define ENEMY_CHASER_NOTICE_DISTANCE 23.0f
    #define ENEMY_CHAER_MOVE_SPEED 30.0f
    #define ENEMY_TURRET_BULLET_SPEED 100.0f

    #define ENEMY_SIZE 0.30f
}

//Check whether health is below 0 or not.
inline bool enemy::HealthCheck(void)
{
    if(Health <= 0)
    {
        Visible = false;
        Bullet.Visible = false;
    }
    return Visible;
}

//Initialize enemy. Used while loading the level.
void enemy::Initialize(point InitPos, float TileWidth, float TileHeight)
{
    Pos = InitPos;
    Width = ENEMY_SIZE * TileWidth;
    Height = ENEMY_SIZE * TileHeight;
    float HeightGrow = 0.2f * TileHeight;
	Pos.Y += HeightGrow;
    Health = ENEMY_CHASER_HEALTH;
    
    Visible = true;
    id = -1;
    AttackCoolDown = ENEMY_ATTACK_COOLDOWN;
    Bullet.Initialize(0.5f * Width, 0.5f * Height, id, 2.0f);
    Bullet.Visible = false;
}

//Initialize enemy_chaser. Used while loading the level.
void enemy_chaser::Initialize(point InitTilePos, 
                              float InputTileWidth, float InputTileHeight,
                              int _CountX, int _CountY,
                              point DownLeftPos)
{
    Pos.X = (InitTilePos.X - _CountX / 2) * InputTileWidth;
	Pos.Y = (InitTilePos.Y - _CountY / 2) * InputTileHeight;

    float HeightGrow = 0.15f * InputTileHeight;
    Pos.Y += HeightGrow;

    Width = ENEMY_SIZE * InputTileWidth;
    Height = ENEMY_SIZE * InputTileHeight + 2 * HeightGrow;;
    Health = ENEMY_CHASER_HEALTH;
    
    State = 1;
    Visible = true;
    id = -1;
    AttackCoolDown = ENEMY_ATTACK_COOLDOWN;
    Bullet.Initialize(0.8f * Width, 0.8f * Height, id, 2.0f);    
    Bullet.Visible = false;

    dX = 0.0f;
    dY = 0.0f;
    DirX = 1;
    AttackDestiny = {0.0f, 0.0f};

    //SFX
    int SoundRandomSelector = WERandom::GetInt(-1, 2);
    switch(SoundRandomSelector)
    {
        default:
        case 0:
            EnemySFX.MakeSound("Sounds\\SFX\\enemy_death1.wav", false);
            break;
        case 1:
            EnemySFX.MakeSound("Sounds\\SFX\\enemy_death2.wav", false);
            break;
        case 2:
            EnemySFX.MakeSound("Sounds\\SFX\\enemy_death3.wav", false);
            break;            
    }
    DeathSoundPlayed = false;    
}

//Initialize enemy_chaser. Used while loading the level.
void enemy_turret::Initialize(point InitTilePos, 
                              float InputTileWidth, float InputTileHeight,
                              int _CountX, int _CountY,
                              point DownLeftPos)
{
    Pos.X = (InitTilePos.X - _CountX / 2) * InputTileWidth;
	Pos.Y = (InitTilePos.Y - _CountY / 2) * InputTileHeight;

    float HeightGrow = 0.15f * InputTileHeight;
    Pos.Y += HeightGrow;
    Width = ENEMY_SIZE * InputTileWidth;
    Height = ENEMY_SIZE * InputTileHeight + 2 * HeightGrow;

    Health = ENEMY_TURRET_HEALTH;
    
    Visible = true;
    id = -2;
    AttackCoolDown = ENEMY_ATTACK_COOLDOWN;
    Bullet.Initialize(0.5f * Width, 0.5f * Height, id, 2.0f);
    
    dX = 0.0f;
    dY = 0.0f;
    Bullet.Visible = false;
}

//Reset the bullet to the creator's position.
void enemy::ResetBullet(void)
{
    Bullet.Visible = true;
    Bullet.Pos = Pos;
    AttackCoolDown = ENEMY_ATTACK_COOLDOWN;
}

//enemy_chaser's normal move when the player is too far away.
//In this case, velocity is constant
void enemy_chaser::Move(float dt)
{
    Pos.X += ENEMY_CHAER_MOVE_SPEED * dX * dt;
    Pos.Y += ENEMY_CHAER_MOVE_SPEED * dY * dt;
    
}

//enemy_chaser's move when the player is within certian distance.
void enemy_chaser::AttackMove(point Destiny, float dt, tile_map *CurrentTileMap)
{
    point NewPos = Pos;
    NewPos.X += ENEMY_CHASER_DASH_SPEED * (Destiny.X - Pos.X) * dt;
    NewPos.Y += ENEMY_CHASER_DASH_SPEED * (Destiny.Y - Pos.Y) * dt;

    int NewChaserTileX = 
        RoundFloatToInt((NewPos.X + CurrentTileMap->TileWidth * CurrentTileMap->CountX * 0.425f) / 
                        CurrentTileMap->TileWidth);
    int NewChaserTileY = 
        RoundFloatToInt((NewPos.Y + CurrentTileMap->TileHeight * CurrentTileMap->CountY * 0.4f) / 
                        CurrentTileMap->TileHeight);

    if(CurrentTileMap->Tiles[NewChaserTileY * CurrentTileMap->CountX + NewChaserTileX] != 1)
    {
        Pos = NewPos;
    }
}

//Process enemy_chaser
void enemy_chaser::EnemyChaserProcess(object *Player, float dt, tile_map *CurrentTileMap, float SFXVolume)
{
    
    if(HealthCheck())
    {   
        float Distance = (float)sqrt((Pos.X - Player->Pos.X) * (Pos.X - Player->Pos.X) + 
                                (Pos.Y - Player->Pos.Y) * (Pos.Y - Player->Pos.Y));        
        if(Distance <= ENEMY_CHASER_NOTICE_DISTANCE && State == 1)
        {
            State = 2;
            AttackCoolDown = ENEMY_ATTACK_COOLDOWN;
            point MovingVector = UnitVectorCalculation(Pos, Player->Pos);
            AttackDestiny.X = Pos.X + ENEMY_CHASER_DASH_LENGTH * MovingVector.X;
            AttackDestiny.Y = Pos.Y + ENEMY_CHASER_DASH_LENGTH  * (Height / Width) * MovingVector.Y;
            Bullet.Visible = true;
        }

        if(State == 1)
        {
            point MovingVector = UnitVectorCalculation(Pos, Player->Pos);
            dX = MovingVector.X;
            dY = MovingVector.Y;
        }
        else
        {
            if(AttackCoolDown >= 0.0f)
            {
                AttackCoolDown -= dt;
            }
            else
            {
                Bullet.Visible = false;
                State = 1;
            }
        }

        if(State == 1)
        {
            Move(dt);
            if(Player->Pos.X >= Pos.X)
                DirX = 1;
            else
                DirX = -1;
        }
        else
        {
            AttackMove(AttackDestiny, dt, CurrentTileMap);
            Bullet.CollideCheck(Player); 
        }
        Bullet.Pos = Pos;            
    }
    else
    {
        if(!DeathSoundPlayed)
        {
            EnemySFX.channel->setVolume(0.005f * SFXVolume);
            EnemySFX.PlaySound();
            DeathSoundPlayed = true;
        }
    }
}

//process enemy_turret
void enemy_turret::EnemyTurretProcess(object *Player, float dt)
{
    if(HealthCheck())
    {            
        if(AttackCoolDown <= 0.0f)
        {
            ResetBullet();

            point Offset = {12.0f, 12.0f};

            point BulletVector = UnitVectorCalculation(Pos, Player->Pos);

            Bullet.Activate(Pos, 
                                ENEMY_TURRET_BULLET_SPEED * BulletVector.X, 
                                ENEMY_TURRET_BULLET_SPEED * BulletVector.Y);
        }
        else
        {
            Bullet.Move(dt);
            Bullet.CollideCheck(Player);            

            AttackCoolDown -= dt;
        }
    }
}