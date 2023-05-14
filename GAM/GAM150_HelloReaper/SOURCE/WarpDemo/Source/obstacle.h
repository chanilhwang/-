/******************************************************************************
FileName: obstacle.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "hreaper_general.h"

class obstacle
{
public:
    point Pos;

    float Width;
    float Height;

    int State;

    float MovingTime;
    float WaitingTime;
    float AttackingTime;

    bool Attacked;

    void Initialize(point InitTilePos,
                    float TileWidth, float TileHeight,
                    int _CountX, int _CountY);
};

class obstacle_chaser : public obstacle
{
public:
    void ObstacleChaserProcess(object *, float);
};

#endif