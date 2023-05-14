/******************************************************************************
FileName: obstacle.cpp
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "obstacle.h"
#include "collision.h"
namespace
{
    #define OBSTACLE_CHASER_MOVING_TIME 0.7f
    #define OBSTACLE_CHASER_WAITING_TIME 3.0f
    #define OBSTACLE_CHASER_ATTACKING_TIME 0.3f
    #define OBSTACLE_CHASER_SPEED 35.0f    
}

void obstacle::Initialize(point InitTilePos, 
                                float InputTileWidth, float InputTileHeight,
                                int _CountX, int _CountY)
{
    Pos.X = (InitTilePos.X - _CountX / 2) * InputTileWidth;
	Pos.Y = (InitTilePos.Y - _CountY / 2) * InputTileHeight;

    Width = 0.5f * InputTileWidth;
    Height = 0.5f * InputTileHeight;

    State = 0;
    MovingTime = 0.0f;
    WaitingTime = 0.0f;

    Attacked = false;
}

void obstacle_chaser::ObstacleChaserProcess(object *Player, float dt)
{
    if(State == 0)  //Wait
    {
        if(WaitingTime >= 0.0f)
        {
            WaitingTime -= dt;
        }
        else
        {
            State = 1;        
            MovingTime = OBSTACLE_CHASER_MOVING_TIME;    
        }
    }
    else if(State == 1) //Move
    {
        if(MovingTime >= 0.0f)
        {
            float dY = 0.0f;
            if(Player->Pos.Y - Pos.Y >= 1.0f)
            {
                dY = 1.0f;
            }
            else if(Player->Pos.Y - Pos.Y < -1.0f)
            {
                dY = -1.0f;
            }
            Pos.Y += dY * OBSTACLE_CHASER_SPEED * dt;
            MovingTime -= dt;

            float AbsDistance = Player->Pos.Y - Pos.Y;
            if(AbsDistance < 0.0f)
                AbsDistance *= -1.0f;
            if(AbsDistance <= 1.0f)
            {
                State = 2;
                AttackingTime = OBSTACLE_CHASER_ATTACKING_TIME;                
            }
        }
        else
        {
            State = 2;
            AttackingTime = OBSTACLE_CHASER_ATTACKING_TIME;
        }
    }
	else if (State == 2)
	{
        if(!Attacked && AttackingTime <= 0.7f * OBSTACLE_CHASER_ATTACKING_TIME)
        {
            if(RayCastR(Pos, {1.0f, 0.0f}, *Player) || RayCastR(Pos, {-1.0f, 0.0f}, *Player))
            {
                Player->Health -= 1;
            }          

            Attacked = true;
        }
        if(AttackingTime < 0.0f)
        {
            State = 0;
            Attacked = false;
            WaitingTime = OBSTACLE_CHASER_WAITING_TIME;            
        }
        else
        {
            AttackingTime -= dt;
        }
	}
}