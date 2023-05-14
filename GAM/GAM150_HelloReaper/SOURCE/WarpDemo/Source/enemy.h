/******************************************************************************
FileName: enemy.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef ENEMY_H

#include "hreaper_general.h"
#include "bullet.h"
#include "tile_info.h"
#include "fmod_general.h"

class enemy : public object
{
public:
    float AttackCoolDown;

    bullet Bullet;   

    void Initialize(point InitPos, float TileWidth, float TileHeight);
    bool HealthCheck(void);
    void ResetBullet(void);
};

class enemy_chaser : public enemy
{
public:
    point AttackDestiny;
    int State;
    int DirX;

    fmod_sound_system EnemySFX;
    bool DeathSoundPlayed;
    
    //State 1 : Chasing
    //State 2 : Attacking
    void Initialize(point InitTilePos, 
                    float TileWidth, float TileHeight, 
                    int _CountX, int _CountY,
                    point DownLeftPos);
    void Move(float);
    void AttackMove(point, float, tile_map *);
    void EnemyChaserProcess(object *Player, float dt, tile_map *, float SFXVolume);
};

class enemy_turret : public enemy
{
public:
    void Initialize(point InitTilePos, 
                    float TileWidth, float TileHeight, 
                    int _CountX, int _CountY,
                    point DownLeftPos);
    void ResetBullets(void);                    
    void EnemyTurretProcess(object *Player, float dt);
};

#define ENEMY_H
#endif